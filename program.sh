echo "Compiling serial.c"
gcc -g -Wall -o llist_serial llist_serial.c
echo "Compiling parallel_mutex.c"
gcc -g -Wall -o llist_mutex llist_mutex.c -lpthread -lm
echo "Compiling parallel_rwlock.c"
gcc -g -Wall -o llist_rw llist_rw.c -lpthread -lm

echo "Running configurations.."
export CONF="1000 10000 .99 .005 .005 100";
./llist_serial $CONF
./llist_mutex 1 $CONF
./llist_mutex 2 $CONF
./llist_mutex 4 $CONF
./llist_mutex 8 $CONF 
./llist_rw 1 $CONF
./llist_rw 2 $CONF  
./llist_rw 4 $CONF 
./llist_rw 8 $CONF 

export CONF="1000 10000 .9 .05 .05 100";
./llist_serial $CONF
./llist_mutex 1 $CONF 
./llist_mutex 2 $CONF
./llist_mutex 4 $CONF
./llist_mutex 8 $CONF 
./llist_rw 1 $CONF  
./llist_rw 2 $CONF  
./llist_rw 4 $CONF 
./llist_rw 8 $CONF 

export CONF="1000 10000 .5 .25 .25 100";
./llist_serial $CONF
./llist_mutex 1 $CONF 
./llist_mutex 2 $CONF
./llist_mutex 4 $CONF
./llist_mutex 8 $CONF 
./llist_rw 1 $CONF  
./llist_rw 2 $CONF  
./llist_rw 4 $CONF 
./llist_rw 8 $CONF