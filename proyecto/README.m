# Instrunctions

1. Run make (or make rpi4)
1. sudo ./main_ubuntu 100 1 1 0.1 1//runtinem[ms] log prioinh correctionOffset deadlineTolerance(or main_rpi4)
1. sudo ./main_rpi4   100 1 1 2.5 1//runtinem[ms] log prioinh correctionOffset deadlineTolerance(or main_rpi4)
1. python3 log_analysis.py
1. . clearlogs.sh to clear logs (make clean will also clear logs)
1. quick.sh cleans and runs quickly in a single command

or use quick.sh

correction offset is around 0.1ms for desktop and 2.5ms for rpi4
deadline tolerance is set to 1ms