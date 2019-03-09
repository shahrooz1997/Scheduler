

cache_types = ["NoCache", "Ideal", "Alloy", "HMA", "Banshee", "UnisonCache", "MemCacheAlloy", "MemCacheBanshee", "MemCacheUnison", "FullMem", "DatasetSize"]

benchmarks = ['1_bfs', '2_pr', '3_cc', '4_bc', '5_sssp', '6_tc', '1_bwaves', '2_gems', '4_mcf', '5_milc', '7_mix']

# extensions = ['5', '15', '25', '35', '45', '55', '65', '75', '85', '95']
# extensions = ['10', '20', '30', '40', '50', '60', '70', '80', '90']
# extensions = ['5', '10', '15', '20', '25', '30', '35', '40', '45', '50', '55', '60', '65', '70', '75', '80', '85', '90', '95']
extensions = ['5']

f = open("tasks_generated.txt","w+")

def get_number(cache_type_number, extension_number, benchmark_number):
	return str(cache_type_number + 10) + str(extension_number + 10) + str(benchmark_number + 10)

for i in range(len(cache_types)):
	if(i == 0):
		for j in range(len(extensions)):
			if(j % 2 == 0):
				for k in range(len(benchmarks)):
					f.write(get_number(i, j, k) + " " + cache_types[i] + " " + benchmarks[k] + " " + extensions[j] + "\n")

f.close()
