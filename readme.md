# Generalized K-means

## Dependency
- [Eigen](http://eigen.tuxfamily.org/)
- [Spectral](https://spectralib.org/)

Download them and add their path to your `CPLUS_INCLUDE_PATH`.

## Run Experiments
```
$ ruby exact_vs_randomized.rb
$ ruby gkmeans_vs_gkmeanspp.rb
```
etc...

## Dataset Configuration
Edit `conf/`.
You can see all the configurations used in the thesis from `saved_results/**/conf/`.

A conf file for a stochastic block model looks like

```
4 # number of the blocks
2500 3000 3500 4000 # the sizes of the blocks
0.01 0.001 # p_in and p_out (see the thesis for detail)
1 5 # the weights of inner-block edges and between-block edges
```
