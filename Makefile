.PHONY: push

#	nvidia-geforce-gtx690
#	gzh091010

push:
	git pull
	git add docs
	git add font
	git add images
	git add src
	git add test
	git add .gitconfig
	git add LICENSE
	git add Readme.md
	git commit -m "first commit"
	git push