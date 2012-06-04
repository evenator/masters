# author: Amand Tihon
# email: amand.tihon@alrj.org
# dist-license: GPL3, http://www.gnu.org/licenses/gpl-3.0.txt
# use-license: unlimited


Element["" "" "C000" "" 30000 30000 17500 -20000 0 100 ""]
(
	Pin[10000 0 8000 2000 8600 1500 "2" "2" ""]
	Pin[-10000 0 8000 2000 8600 1500 "1" "1" ""]
	ElementLine [-12500 -10000 -12500 -5000 1500]
	ElementLine [-15000 -7500 -10000 -7500 1500]
	ElementLine [2500 -7500 2500 7500 1500]
	ElementLine [-2500 0 -10000 0 1500]
	ElementLine [2500 0 10000 0 1500]
	ElementLine [0 -7500 0 7500 1500]
	ElementLine [0 7500 -2500 7500 1500]
	ElementLine [-2500 7500 -2500 -7500 1500]
	ElementLine [-2500 -7500 0 -7500 1500]
	ElementArc [0 0 20000 20000 270 90 1500]
	ElementArc [0 0 20000 20000 180 90 1500]
	ElementArc [0 0 20000 20000 90 90 1500]
	ElementArc [0 0 20000 20000 0 90 1500]

	)
