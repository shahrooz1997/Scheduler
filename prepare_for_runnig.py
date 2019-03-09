import shutil
import os
import errno
import sys

benchmarks = []

######### Definitions #########

# On GAPBS
benchmarks.extend(['1_bfs', '2_pr', '3_cc', '4_bc', '5_sssp', '6_tc'])

# On SPEC2006
benchmarks.extend(['1_bwaves', '2_gems', '4_mcf', '5_milc', '7_mix'])

GAPBS_address = '/Disk3TB/kasraa/zsim/gapbs'

graph = "twitter"
# graph = "web"
# graph = "urand"



spec_benchmarks = ['1_bwaves', '2_gems', '4_mcf', '5_milc', '7_mix']

def copy(src, dest):
	try:
		shutil.copytree(src, dest)
	except OSError as e:
		# If the error was caused because the source wasn't a directory
		if e.errno == errno.ENOTDIR:
			shutil.copy(src, dest)
		else:
			print('Directory not copied. Error: %s' % e)

def rm_dir(dirName):
	try:
		shutil.rmtree(dirName)
	except OSError as e:
		pass #print("The directory doesn't exists.")

def rm_file(file_name):
	try:
		os.remove(file_name)
	except OSError as e:
		pass #print("The directory doesn't exists.")

def mkdir(dirName):
	if not os.path.exists(dirName):
		os.mkdir(dirName)
		# print("Directory " + dirName +  " Created ")
	else:
		print("Directory "+ dirName + " already exists")

def what_is_processes_for(benchmark):
	processes = ""
	if(benchmark == "1_bwaves"):
		for i in range(16):
			processes += "process" + str(i) + " = { "
			processes += "command = \"./bwaves_base.i386-m32-gcc42-nn\";"
			processes += " };\n"
		return processes
	elif(benchmark == "2_gems"):
		for i in range(16):
			processes += "process" + str(i) + " = { "
			processes += "command = \"./GemsFDTD_base.i386-m32-gcc42-nn\";"
			processes += " };\n"
		return processes
	elif(benchmark == "4_mcf"):
		for i in range(16):
			processes += "process" + str(i) + " = { "
			processes += "command = \"./mcf_base.i386-m32-gcc42-nn inp.in\";"
			processes += " };\n"
		return processes
	elif(benchmark == "5_milc"):
		for i in range(16):
			processes += "process" + str(i) + " = { "
			processes += "command = \"./milc_base.i386-m32-gcc42-nn\"; input = \"su3imp.in\";"
			processes += " };\n"
		return processes
	elif(benchmark == "7_mix"):
		for i in range(16):
			if(i % 4 == 0):
				processes += "process" + str(i) + " = { "
				processes += "command = \"./bwaves_base.i386-m32-gcc42-nn\";"
				processes += " };\n"
			if(i % 4 == 1):
				processes += "process" + str(i) + " = { "
				processes += "command = \"./GemsFDTD_base.i386-m32-gcc42-nn\";"
				processes += " };\n"
			if(i % 4 == 2):
				processes += "process" + str(i) + " = { "
				processes += "command = \"./mcf_base.i386-m32-gcc42-nn inp.in\";"
				processes += " };\n"
			if(i % 4 == 3):
				processes += "process" + str(i) + " = { "
				processes += "command = \"./milc_base.i386-m32-gcc42-nn\"; input = \"su3imp.in\";"
				processes += " };\n"
		return processes
	elif(benchmark == "1_bfs"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/bfs -f " + GAPBS_address + "/benchmark/graphs/" + graph + ".sg -n64\";"
		processes += " };\n"
		return processes;
	elif(benchmark == "2_pr"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/pr -f " + GAPBS_address + "/benchmark/graphs/" + graph + ".sg -i1000 -t1e-4 -n16\";"
		processes += " };\n"
		return processes;
	elif(benchmark == "3_cc"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/cc -f " + GAPBS_address + "/benchmark/graphs/" + graph + ".sg -n16\";"
		processes += " };\n"
		return processes;
	elif(benchmark == "4_bc"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/bc -f " + GAPBS_address + "/benchmark/graphs/" + graph + ".sg -i4 -n16\";"
		processes += " };\n"
		return processes;
	elif(benchmark == "5_sssp"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/sssp -f " + GAPBS_address + "/benchmark/graphs/" + graph + ".wsg -n64 -d2\";"
		processes += " };\n"
		return processes;
	elif(benchmark == "6_tc"):
		processes += "process" + str(0) + " = { "
		processes += "command = \"" + GAPBS_address + "/tc -f " + GAPBS_address + "/benchmark/graphs/" + graph + "U.sg -n3\";"
		processes += " };\n"
		return processes;


def make_cfg_file(benchmark):
	config_text = ""
	with open(sys.argv[1] + "/" + sys.argv[1] + ".cfg", "r") as ins:
		lines = []
		for line in ins:
			if "memory_fraction" in line:
				tt = long(sys.argv[3])
				tt2 = str(tt / 100.)
				tt2 = tt2[0:4]
				line = "\t\tmemory_fraction = " + tt2 + ";\n"
			if "process0" in line:
				break;
			lines.append(line)

	lines = ''.join(lines)
	processes = what_is_processes_for(benchmark)

	file = open('./prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + '/' + benchmark + '/' + sys.argv[1] + ".cfg", 'w')

	file.write(lines + processes)

	file.close()


if(os.path.isdir(sys.argv[1] + '/src') == False):
	print("Directory src doesn't exists.")
	exit(-1)

if(os.path.isfile(sys.argv[1] + "/" + sys.argv[1] + ".cfg") == False):
	print("The file " + sys.argv[1] + ".cfg" + "doesn't exists.")
	exit(-1)

if sys.argv[2] not in benchmarks:
	print("The benchmark " + sys.argv[1] + " isn't supported")
	exit(-2)

rm_dir("./prepared_files/" + sys.argv[1] + "_" + sys.argv[3])
rm_file("./prepared_files/" + sys.argv[1] + "_" + sys.argv[3] + ".tar.gz")

mkdir("./prepared_files/" + sys.argv[1] + "_" + sys.argv[3])

copy('./prepare_files/banshee-master', './prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + '/banshee-master')
copy('./' + sys.argv[1] + '/src', './prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + '/banshee-master/src')


benchmark = sys.argv[2]

if benchmark in spec_benchmarks:
	copy('./prepare_files/' + benchmark, './prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + '/' + benchmark)
else:
	mkdir('./prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + '/' + benchmark)

make_cfg_file(benchmark)

copy("./prepare_files/runner.py", './prepared_files/' + sys.argv[1] + "_" + sys.argv[3] + "/runner.py")

os.system("cd prepared_files; tar -czvf " + "./" + sys.argv[1] + "_" + sys.argv[3] + ".tar.gz " + "./" + sys.argv[1] + "_" + sys.argv[3] + " >/dev/null 2>&1")

rm_dir("./prepared_files/" + sys.argv[1] + "_" + sys.argv[3])

print("The required file was created in prepared_files directory.")
