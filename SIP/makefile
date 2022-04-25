libtest.so :SIPUA_Stack.cpp
	g++ c.cpp -losip2 -leXosip2 -lpthread -fPIC -shared -o libtest.so
	cp ./libtest.so ./tests/
	cp ./c.h ./tests/

clean:
	rm libtest.so