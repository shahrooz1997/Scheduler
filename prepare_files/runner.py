import os
import sys
import datetime

server_name = sys.argv[1]
cache_type = sys.argv[2]
benchmark = sys.argv[3]
cache_type_name_extension = sys.argv[4]

def there_is_error(num):
	date_time = list(str(datetime.datetime.now()))
	for i in range(len(date_time)):
		if(date_time[i] == " "):
			date_time[i] = "_"
			break;

	date_time = "".join(date_time)

	os.system("ssh -t zard \"cd /Disk3TB/shahrooz/; touch error_on_" + server_name + "_" + "server" + "_" + cache_type + "_" + cache_type_name_extension + "_" + benchmark + "_" + str(num) + "_" + date_time + "\"")


def info(text):

	text = list(text)
	for i in range(len(text)):
		if(text[i] == " "):
			text[i] = "_"

	text = "".join(text)

	date_time = list(str(datetime.datetime.now()))
	for i in range(len(date_time)):
		if(date_time[i] == " "):
			date_time[i] = "_"
			break;

	date_time = "".join(date_time)

	os.system("ssh -t zard \"cd /Disk3TB/shahrooz/info; touch " + server_name + "_" + "server" + "_" + cache_type + "_" + cache_type_name_extension + "_" + benchmark + "_" + str(text) + "_" + date_time + "\"")

os.system("ssh -t zard mv /Disk3TB/shahrooz/status/idle/" + server_name + " /Disk3TB/shahrooz/status/running/")

info("started")

os.system("cd banshee-master; scons -j 9 >zsim_compiling_stdout.txt 2>zsim_compiling_stderr.txt")

with open('./banshee-master/zsim_compiling_stdout.txt', 'r') as f:
    lines = f.read().splitlines()
    last_line = lines[-1]
    if(last_line != "scons: done building targets."):
    	there_is_error(1)
    	os.system("ssh -t zard mv /Disk3TB/shahrooz/status/running/" + server_name + " /Disk3TB/shahrooz/status/idle/")
    	exit(-1)

info("built")

print("Make of zsim is completed")

os.system("cd " + benchmark + "; ../banshee-master/build/opt/zsim " + cache_type + ".cfg >zsim_stdout.txt 2>zsim_stderr.txt")
if(os.path.isfile("./" + benchmark + "/zsim.h5") == False):
	there_is_error(2)
	os.system("ssh -t zard mv /Disk3TB/shahrooz/status/running/" + server_name + " /Disk3TB/shahrooz/status/idle/")
	exit(-1)

print("The running of zsim is completed")

os.system("ssh -t zard mkdir /Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension)
os.system("ssh -t zard mkdir /Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension + "/" + benchmark)
os.system("scp ./" + benchmark + "/zsim.h5 zard:/Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension + "/" + benchmark)
os.system("scp ./" + benchmark + "/zsim_stdout.txt zard:/Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension + "/" + benchmark)
os.system("scp ./" + benchmark + "/zsim_stderr.txt zard:/Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension + "/" + benchmark)
os.system("scp ./" + benchmark + "/out.cfg zard:/Disk3TB/shahrooz/RES/Res_" + cache_type + "_" + cache_type_name_extension + "/" + benchmark)
os.system("ssh -t zard mv /Disk3TB/shahrooz/status/running/" + server_name + " /Disk3TB/shahrooz/status/idle/")

info("finished")

os.system("rm -rf ../" + cache_type + "_" + cache_type_name_extension + " ../" + cache_type + "_" + cache_type_name_extension + ".tar.gz")
