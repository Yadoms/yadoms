#!/usr/bin/env python

# Test remapping of topic name for outgoing message

import socket

import inspect, os, sys
# From http://stackoverflow.com/questions/279237/python-import-a-module-from-a-folder
cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"..")))
if cmd_subfolder not in sys.path:
    sys.path.insert(0, cmd_subfolder)

import mosq_test

rc = 1
keepalive = 60
client_id = socket.gethostname()+".bridge_sample"
connect_packet = mosq_test.gen_connect(client_id, keepalive=keepalive, clean_session=False, proto_ver=128+3)
connack_packet = mosq_test.gen_connack(rc=0)

client_connect_packet = mosq_test.gen_connect("pub-test", keepalive=keepalive)
client_connack_packet = mosq_test.gen_connack(rc=0)

ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ssock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
ssock.settimeout(4)
ssock.bind(('', 1888))
ssock.listen(5)

broker = mosq_test.start_broker(filename=os.path.basename(__file__), port=1889)


def test(bridge, sock):
    if not mosq_test.expect_packet(bridge, "connect", connect_packet):
        return 1

    bridge.send(connack_packet)

    cases = [
        ('local/topic/something', 'remote/topic/something'),
        ('local/topic/some/t/h/i/n/g', 'remote/topic/some/t/h/i/n/g'),
        ('local/topic/value', 'remote/topic/value'),
        # Don't work, #40 must be fixed before
        # ('local/topic', 'remote/topic'),
        ('local2/topic/something', None),  # don't match topic pattern
        ('local2/topic/prefix/something', 'remote2/topic/prefix/something'),
        ('local3/topic/something/value', 'remote3/topic/something/value'),
        ('local4/topic/something', 'remote4/tipic/something'),
        ('local5/topic/something', None),
    ]

    mid = 3
    for (local_topic, remote_topic) in cases:
        mid += 1
        local_publish_packet = mosq_test.gen_publish(
            local_topic, qos=0, mid=mid, payload=''
        )
        sock.send(local_publish_packet)
        if remote_topic:
            remote_publish_packet = mosq_test.gen_publish(
                remote_topic, qos=0, mid=mid, payload=''
            )
            match = mosq_test.expect_packet(bridge, "publish", remote_publish_packet)
            if not match:
                print("Fail on cases local_topic=%r, remote_topic=%r" % (
                    local_topic, remote_topic,
                ))
                return 1
        else:
            bridge.settimeout(3)
            try:
                bridge.recv(1)
                print("FAIL: Received data when nothing is expected")
                print("Fail on cases local_topic=%r, remote_topic=%r" % (
                    local_topic, remote_topic,
                ))
                return 1
            except socket.timeout:
                pass
            bridge.settimeout(20)
    return 0

try:
    (bridge, address) = ssock.accept()
    bridge.settimeout(2)

    sock = mosq_test.do_client_connect(
        client_connect_packet, client_connack_packet,
        port=1889,
    )

    rc = test(bridge, sock)

    sock.close()
    bridge.close()
finally:
    try:
        bridge.close()
    except NameError:
        pass

    broker.terminate()
    broker.wait()
    if rc:
        (stdo, stde) = broker.communicate()
        print(stde)
    ssock.close()

exit(rc)
