clear
g++ *.cpp -c -fPIC
g++ *.o -o libtest.so -fPIC -shared -losip2 -leXosip2 -lpthread
rm *.o