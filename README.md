 # Generate Keys

```sh
openssl dhparam -out etc/server/keys/dh.pem 2048
openssl req -newkey rsa:2048 -nodes -keyout etc/server/keys/key.pem -x509 -days 10000 -out etc/server/keys/cert.pem -subj "//C=US\ST=CA\L=Los Angeles\O=Beast\CN=localhost"
```
