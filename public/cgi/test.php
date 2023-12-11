<?php
ini_set('output_buffering', 'off');
ini_set('zlib.output_compression', false);
ini_set('implicit_flush', true);
ob_implicit_flush(true);

$method = getenv('REQUEST_METHOD');
$scriptName = getenv('SCRIPT_NAME');
$queryString = getenv('QUERY_STRING');

echo "Content-Type: text/html\r\n";
echo "\r\n";

echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<title>CGI with PHP</title>";
echo "</head>";
echo "<body>";
echo "<h1>CGI Script Information</h1>";
echo "<p>Method: $method</p>";
echo "<p>Script Name: $scriptName</p>";
echo "<p>Query String: $queryString</p>";
echo "</body>";
echo "</html>";

flush();
?>
