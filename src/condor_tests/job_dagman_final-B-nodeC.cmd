executable = ./job_dagman_final-B-node.pl
output = job_dagman_final-B-nodeC.out
error = job_dagman_final-B-nodeC.err
log = job_dagman_final-B-nodeC.log
arguments = "'OK done with C' 0 $(DAG_SUCCESS)"
universe = scheduler
queue
