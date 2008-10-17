for CONFIG in \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=1 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=1 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=1 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=1 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=1 -DREENTER=0 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=1 -DSTATIC=0 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=1 -DQUICK=0" \
	"-DDEBUG=0 -DUNOPTIMISED=0 -DPROFILE=0 -DSMALL=0 -DINFO=0 -DREENTER=0 -DSTATIC=0 -DQUICK=1"
do
	cmake $CONFIG .
	make clean
	make minion $@
	if [ $? -ne 0 ]; then
		echo "Compilation failed for configuration $CONFIG."
		break
	fi
done
