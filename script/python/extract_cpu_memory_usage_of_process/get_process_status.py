import psutil, sys

if len(sys.argv) != 2:
	print 'No Process name!!!'
	sys.exit(0);

process_name = sys.argv[1]
pid_no = -1
while True:
	for onePid in psutil.get_pid_list():
		if psutil.Process(onePid).name == process_name:
			pid_no = onePid
			break
	if pid_no != -1:
		break

log_file_stream = open('%s.%s' % (process_name, pid_no), 'w')
p = psutil.Process(pid_no)

while True: 
	try:
		cpu = p.get_cpu_percent(interval=1)
		rss, vms = p.get_memory_info()
		log_file_stream.write('%s\t%s\t%s\n' % (cpu, vms, rss))
		log_file_stream.flush()
	except:
		break

log_file_stream.close()
