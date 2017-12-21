# Simplescalar subblock cache simulator

- This is a functional cache simulator for subblocks.
- subblocks cache's retain a single copy of tag for the entire superblock, hence reducing the required tag area.
- Full Eviction LRU and Least Agrigated entry eviction are the two replacement policies implemented in this simulator. 

### The original cache simualtor can be found here
- [Simplescalar](http://www.simplescalar.com/) 
### Instructions
1) Git clone the repo
```sh
$ git clone https://github.com/shrivinayak/sub-block-cache-simulator.git
```
2) Compile the simulator
```sh
$ cd sub-block-cache-simulator/
$ make sim-cache
```
3) Run the simulator
```sh
$ ./sim-cache -cache:il1 il1:8:16:4:e -cache:il2 dl2 -cache:dl1 dl1:8:16:4:e -cache:dl2 unified:16:16:4:e <application>
```
### Note:
> Application should be compiled using simplescalar gcc cross compiler which can be obtained from the official website

> This simulator has two different replacement policies. Use l for FE-LRU and e for LAEE. 

> cache parameters are passed on the commandline as shown above. For more details on how to configure cache, refer the official guide or the [hack-guide](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwjz18_upJrYAhVU5GMKHQxjAasQFggpMAA&url=http%3A%2F%2Fwww.simplescalar.com%2Fdocs%2Fhack_guide_v2.pdf&usg=AOvVaw2KFrJT_cYT0eXr7L4R9uGt).

