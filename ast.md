server
|__ {
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