server
    |__ listen 
        |__ 8000;
    |__ server_name 
        |__ localhost;
    |__ root 
        |__ www;
    |__ cgi 
        |__ .php 
        |__ php-cgi;
    |__ client_max_body_size 
        |__ 1000;
    |__ error_page 
        |__ 404 
        |__ ./www/errors/404.html;
    |__ location 
        |__ /
        |__ autoindex 
            |__ on;
        |__ index 
            |__ index.html;
        |__ allow_methods 
            |__ GET;

  server
    listen
    8000
    server_name
    localhost
    root
    www
    cgi
    .php
    php-cgi
    client_max_body_size
    1000
    error_page
    404
    ./www/errors/404.html
    location
    /

      autoindex
      on
      index
      index.html
      allow_methods
      GET
    location
    /data

      autoindex
      on
      index
      index.html
      allow_methods
      POST
      GET
      DELETE
    location
    /admin

      autoindex
      off
      auth
      admin:admin
      index
      index.html
    location
    /_uploads

      autoindex
      on
      index
      index.html

server 
    |-- { 
        |-- listen 
            |-- 8000;
        |-- server_name |-- localhost |-- ;
        |__ root |-- www |-- ;
    |__ cgi 
        |__ .php 
        |__ php-cgi;
    |__ client_max_body_size 
        |__ 1000;
    |__ error_page 
        |__ 404 
        |__ ./www/errors/404.html;
    |__ location 
        |__ /data 
        |__ {
            |__ autoindex 
                |__ on;
            |__ index 
                |__ index.html;
            |__ allow_methods 
                |__ GET;
    }
}

LB -> New branch
SEMI -> 