 # Generate Keys

```sh
openssl dhparam -out etc/server/keys/dh.pem 2048
openssl req -newkey rsa:2048 -nodes -keyout etc/networkstream/server/keys/key.pem -x509 -days 10000 -out etc/networkstream/server/keys/cert.pem -subj "//C=US\ST=CA\L=Los Angeles\O=Beast\CN=localhost"
```
