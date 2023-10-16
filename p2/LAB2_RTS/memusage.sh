valgrind --tool=massif --peak-inaccuracy=0.0 --stacks=yes --heap=yes --time-unit=ms --detailed-freq=1 --threshold=0.0 --massif-out-file=Linear.massif ./Linear
valgrind --tool=massif --peak-inaccuracy=0.0 --stacks=yes --heap=yes --time-unit=ms --detailed-freq=1 --threshold=0.0 --massif-out-file=Random.massif ./Random
# Use massif-visualizer massif.out.XXXX or the .massif file above
# and ms_print massif.out.XXXX or the .massif file above