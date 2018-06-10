all:
	cd src; make

release:
	cd src; make release
cscope:
	find . -name '*.cpp' -o -name '*.h' > cscope.files
	cscope -b

clean:
	rm -v bin/release/*
	rm -v bin/debug/*
