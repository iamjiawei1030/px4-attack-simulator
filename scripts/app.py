import os
import sys
import time
import subprocess

src = os.environ.get('SRC_DIR')
env = os.environ.copy()
gpsattack = '/attack/gps'
imuattack = '/attack/imu'

if src == None:
	print('Path to PX4-Autopilot required (SRC_DIR)')
	sys.exit(1)


def launch_simulator():
	print('[*] Starting simulator... Please wait...')
	process = subprocess.Popen(['./launch.sh'],
	                           env=env,
	                           stdout=subprocess.DEVNULL,
	                           stderr=subprocess.DEVNULL)
	time.sleep(10)
	return process


def restart_simulator(simulator):
	if simulator:
		print('[*] Stopping simulator...')
		simulator.terminate()
		return launch_simulator()


def send_signal(attack, signal):
	print(f'[*] Sending signal:{signal} to path:{attack}')
	subprocess.run(['out/bin/command', attack, signal])
	print('[*] Signal sent!!!\n\n')


simulator = launch_simulator()
print('[*] Simulator should be up and running...')

while True:
	print('Cybersecurity Labs!\n')

	print('Please select attack method:\n')
	print('1. GPS Jamming')
	print('2. GPS Spoofing [NOT IMPLEMENTED]')
	print('3. IMU Attack')
	print('3. Control Command Spoofing [NOT IMPLEMENTED]')
	print('4. Control Command Jamming [NOT IMPLEMENTED]')
	print('8. Restart Simulator')
	print('9. Exit Attack Center')

	selection = input('\nEnter Your choice: ')

	os.system('clear')  # ugly method

	if selection == '1':
		send_signal(gpsattack, '1')
	elif selection == '2':
		send_signal(gpsattack, '2')
	elif selection == '3':
		send_signal(imuattack, '1')
	elif selection == '8':
		simulator = restart_simulator(simulator)
		print('[*] Simulator should be up and running...')
	elif selection == '9':
		simulator.terminate()
		sys.exit(0)
	else:
		print('Unknown command')
