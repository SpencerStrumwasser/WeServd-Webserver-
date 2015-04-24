

parser:
	./build.sh
tests:
	./build_tests.sh

clean: clean-parser clean-general

clean-general:
	rm -rf webserver

clean-parser: 
	rm -rf *.dSYM build/* config_parser_test	
