#-------------------------------------------------------------------------------
# Util common functions for CPP generation






#-------------------------------------------------------------------------------
def toEnumValueName(label):
   return "k" + label.strip().replace(" ", "_").replace("+", "plus").replace("-", "_").replace(".", "_").replace(",", "_")