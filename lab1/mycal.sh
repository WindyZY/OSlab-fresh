#! /bin/bash
s=$1
e=$2
for((i=s;i<=e;i++))
do
case $i in
	"1"|"January")
		cal 1 $3;;
	"2"|"February")
		cal 2 $3;;
	"3"|"March")
		cal 3 $3;;
	"4"|"April")
		cal 4 $3;;
	"5"|"May")
		cal 5 $3;;
	"6"|"June")
		cal 6 $3;;
	"7"|"July")
		cal 7 $3;;
	"8"|"August")
		cal 8 $3;;
	"9"|"September")
		cal 9 $3;;
	"10"|"October")
		cal 10 $3;;
	"11"|"November")
		cal 11 $3;;
	"12"|"December")
		cal 12 $3;;
	"")
		echo "Please input month and year!"
esac
done
