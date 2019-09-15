(gci -include *.cpp, *.h, *.hpp -recurse | select-string .).Count
pause