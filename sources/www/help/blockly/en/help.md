<div class="bg-primary">

# <a name="summary"></a>Summary

</div>


## Manage devices

 * [Get a device value](#keyword-value-get)
 * [Set a device value](#keyword-value-set)

## Events

 * [**Wait for events**](#yadoms_wait_for_event)
 * [Event data](#yadoms_wait_for_event_result)

## Actions

 * [Sleep](#yadoms_sleep)
 * [System information](#yadoms_get_info)
 * [Log](#yadoms_log)
 * [Notify](#yadoms_notification_simple)

## Manage date and time

 * [Date and time](#yadoms_date_datetime)
 * [Date](#yadoms_date_date)
 * [Time](#yadoms_date_time)
 * [Weekday](#yadoms_date_weekDay)
 * [Every day](#yadoms_date_everyDay)
 * [Today](#yadoms_date_today)
 * [Now](#yadoms_date_now)
 * [Sunrise](#yadoms_date_sunrise)
 * [Sunset](#yadoms_date_sunset)
 * [Duration](#yadoms_date_timespan)
 * [Date/time arithmetics](#yadoms_date_arithmetic)
 * [Extract information](#yadoms_date_extract)


<div class="bg-primary">

# <a name="keyword-value-get"></a>Get a device value

</div>

The following block get a device value.

Only the device selection is required.

![](/this/keyword_get_simple.png)

The block type automatically change to match chose device.

*Example for boolean*

![](/this/keyword_get_boolean.png)

*Example for numeric value*

![](/this/keyword_get_value.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="keyword-value-set"></a>Set a device value

</div>

The following block set a device value.

Only the device selection and the value are required.

![](/this/keyword_set_simple.png)

When chosing a device, the block adapt itself and plug a compatible block.


*Example for a boolean*

![](/this/keyword_set_boolean.png)

*Example for a numeric value*

![](/this/keyword_set_value.png)

*Special case for events*

In some case, device are action commandable.
In such a case, the block change its title

![](/this/keyword_set_action.png)


*[Back to summary](#summary)*


<div class="bg-primary">

# <a name="yadoms_wait_for_event"></a>**Wait for events**

</div>

Block which allow to wait for one or many events to occur.

An event can be one of the following types:
 * on a device data apparition : for example, waiting for the next temperature from my living room
 * on a device data apparition on a condition : for example, waiting for the next temperature greater than 35°C from my living room
 * on a data type apparition : for example, waiting for the next temperature data (from any of all my devices)
 * on a data type apparition on a condition : for example, waiting for the next temperature data greater than 35°C (from any of all my devices)
 * on a specific date and time : for example : do something every tuesday at 10h42 AM
 * every minutes

## Block behavior

When one of configured event occurs, then the block apply associated actions, then **ends**.

In most of cases, its is recommanded to put block into an infinite loop.


## Block configuration

This is to use the gear/cog icon to edit block behavior.
From the opening graphical interface, events can be chosen.

![](/this/wait-for-event-data.gif)

The block can be modulated to wait several events simultaneously.

*Example : block configuration*
![](/this/wait_for_event_data_exemple_multiple.png)

*Result after configuration*
![](/this/wait_for_event_data_exemple_multiple_with_selection.png)


It is also possible to obtain information about the event being processed.

This is accomplished using the block ["Event information"](#yadoms_wait_for_event_result):

It could get:
 * the device name (usefull when a data type is triggered to know which device is event source)
 * the exact value which trigerred the event

*Complete example*

![](/this/wait_for_event_data_exemple_complet.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_wait_for_event_result"></a>Event information

</div>

Obtain information about the current event.

This helps to know which equipment/data triggered the event; as well as its value.

![](/this/wait_for_event_result.png)

*Example*
Here is a script that created the text "The battery level of Weather Station Oregon is low (4%)'

![](/this/wait_for_event_result_exemple.png)

*[Back to summary](#summary)*



<div class="bg-primary">

# <a name="yadoms_sleep"></a>Sleep

</div>

Make the script gogin to sleep for a specific duration.

*Example : wait for 10 seconds*

![](/this/yadoms_sleep.png)

The unit is configurable and allow to wait for some
 * seconds
 * minutes
 * hours
 * days

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_log"></a>Log

</div>

Log a text dat.
The content is stored into the log file associated to the current script.

![](/this/yadoms_log.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_notification_simple"></a>Notify

</div>

Notification somebody

The notification methods are provided by plugins. So dependending on the installation, notification methods can change.
This could be an email, a SMS,...

![](/this/yadoms_notification_simple.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_datetime"></a>Date and time

</div>

Block which define a date and a time.

![](/this/yadoms_date_datetime.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_date"></a>Date

</div>

Block which define only a date.

![](/this/yadoms_date_date.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_time"></a>Hour

</div>

Block which define only a time.

![](/this/yadoms_date_time.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_weekDay"></a>Week day

</div>

Block which define a week day.

![](/this/yadoms_date_weekDay.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_everyDay"></a>Every day

</div>

Block which define any day of the week. (usefull with block ["Wait for events"](#yadoms_wait_for_event))

![](/this/yadoms_date_everyDay.png)

*[Back to summary](#summary)*


<div class="bg-primary">

# <a name="yadoms_date_today"></a>Today

</div>

Block which define the actual day

![](/this/yadoms_date_today.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_now"></a>Now

</div>

Block which define the actual time

![](/this/yadoms_date_now.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_sunrise"></a>Sunrise

</div>

Block which allow to get the sunrise (for current day only)

![](/this/yadoms_date_sunrise.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_sunset"></a>Sunset

</div>

Block which allow to get the sunset (for current day only)

![](/this/yadoms_date_sunset.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_timespan"></a>Duration 

</div>

Block which allow to define a duration.

![](/this/yadoms_date_timespan.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_arithmetic"></a>Date and time arithmetics

</div>

Realizes arithmetic operations within dates and time.

![](/this/yadoms_date_arithmetic.png)

*[Back to summary](#summary)*

<div class="bg-primary">

# <a name="yadoms_date_extract"></a>Extract information 

</div>

Extract a part of a date/time

![](/this/yadoms_date_extract.png)

It is possible to extract:
 * the date without time
 * the complete time (format HH:MM)
 * the year
 * the month
 * the day
 * the hour
 * the minutes
 * the weekday

*[Back to summary](#summary)*

