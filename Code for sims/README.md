# Tiny Code Deploy


## Run on Cluster

1. Edit PredPrey.cpp to take command line arguments in the order that you need
   1. Try to avoid gaps in command line arguments
2. Configure parameterConfig.json file, ready to generate command line arguments file
3. run `python generate.py` to generate command line arguments file
4. Copy src code to cluster
5. Copy slurm config files to cluster
   1. runline
   2. slurm-job.sh
   3. replicates.sh
6. Ensure runline is executable
7. Compile code on cluster
8. Copy binary to home folder
9. Double check paths in slurm-job.sh
10. Submit job with `sbatch ./slurm-job.sh` from home folder

