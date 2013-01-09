# author: Amand Tihon
# email: amand.tihon@alrj.org
# dist-license: GPL3, http://www.gnu.org/licenses/gpl-3.0.txt
# use-license: unlimited


Element["" "Diode, 300 mil spacing" "D000" "" 30000 20000 -7500 -10000 0 100 ""]
(
	Pin[-15000 0 8000 2000 8600 1500 "1" "1" "edge2"]
	Pin[15000 0 8000 2000 8600 1500 "2" "2" "edge2"]
	ElementLine [-7500 -2500 -7500 2500 1500]
	ElementLine [-7500 2500 7500 2500 1500]
	ElementLine [7500 2500 7500 -2500 1500]
	ElementLine [7500 -2500 -7500 -2500 1500]
	ElementLine [7500 0 15000 0 1500]
	ElementLine [-15000 0 -7500 0 1500]
	ElementLine [6000 -1000 6000 1000 3000]
	ElementLine [5000 -2500 5000 2500 1500]

	)
