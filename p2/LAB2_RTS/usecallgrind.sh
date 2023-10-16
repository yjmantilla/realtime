# Run
valgrind --tool=callgrind --dump-instr=yes --cache-sim=yes --instr-atstart=no --callgrind-out-file=Linear.grind ./Linear 
valgrind --tool=callgrind --dump-instr=yes --cache-sim=yes --instr-atstart=no --callgrind-out-file=Random.grind ./Random

# Then use kcachegrind callgrind.out.<pid> or the .grind, opens a gui, then go to the Call Graph tab in the bottom right corner