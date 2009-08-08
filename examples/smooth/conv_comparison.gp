set terminal postscript eps enhanced
set output 'conv_comparison.eps'
set size 0.8, 0.8
set logscale y
set grid
set title 'Error Convergence for the Smooth Problem'
set xlabel 'Degrees of Freedom'
set ylabel 'Error [%]'
plot '-' w lp linetype 1 pointtype 6 title 'exact error p' ,  '-' w lp linetype 2 pointtype 0 title 'exact error h 1' , '-' w lp linetype 3 pointtype 0 title 'exact error h 2' 
1  39.227831035925
9  7.6101484857422
25  0.99319293153097
49  0.097706245502128
81  0.0076983828473046
121  0.00050542643690237
169  2.8432555719711e-05
225  1.3990059976946e-06
289  6.1168336032162e-08
361  2.40633e-09
e
1  39.227831035925
9  18.759087210229
49  9.2860251681937
225  4.6317289872631
961  2.3144664496353
3969  1.1570588544336
16129  0.57850764293309
65025  0.28925109880628
261121 0.144625
e
9  7.6101484857422
49  1.8868872008927
225  0.4699251567634
961  0.11735627041301
3969  0.029331056722878
16129  0.0073322604273087
65025  0.0018330335742496
261121 0.000458256
e
set terminal x11
