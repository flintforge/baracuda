

k=`basename $1 .cu`
nvcc -ptx $1
(( $? )) || ptx2c $k.ptx $k

