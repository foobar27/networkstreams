 # Generate Keys

```sh
openssl dhparam -out share/server/dh.pem 2048
openssl req -newkey rsa:2048 -nodes -keyout share/server/key.pem -x509 -days 10000 -out share/server/cert.pem -subj "//C=US\ST=CA\L=Los Angeles\O=Beast\CN=localhost"
```
