#！/bin/bash
dir=`ls ./Tests_adv/`
for i in $dir
do
	../parser ./Tests_adv/$i
done
