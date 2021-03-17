import os
import sys
import xmltodict
from collections import OrderedDict

src = os.environ.get('SRC_DIR')

if src == None:
	print('Path to PX4-Autopilot required (SRC_DIR)')
	sys.exit(1)

# iris model for now
filename = f'{src}/Tools/sitl_gazebo/models/iris/iris.sdf'
file = open(filename, 'r')
content = file.read()
file.close()

dict = xmltodict.parse(content)

#print(dict['sdf']['model'].keys())

# GPS sensor
#dict['sdf']['model']['model']['link']['sensor']['plugin']['@filename'] = 'path_to_plugin'

#for plugin in dict['sdf']['model']['plugin']:
#	if plugin['@name'] == 'rotors_gazebo_imu_plugin':
#		plugin['@filename'] = 'path_to_plugin'

custom_plugin = OrderedDict([('@name', 'custom_plugin'),
                             ('@filename', 'libcustom_plugin.so'),
                             ('robotNamespace', None)])

dict['sdf']['model']['plugin'].append(custom_plugin)

xml = xmltodict.unparse(dict, pretty=True, full_document=False)

filename = 'worlds/iris.xml'
file = open(filename, 'w')
file.write(xml)
file.close()

print('World Generated')