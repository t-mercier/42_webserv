# Webserv

## Overview

`Webserv` is a custom HTTP server developed by Timothée Mercier and Alfa Saijers as a school project. Implemented in C++, this server is capable of handling basic HTTP requests and serving static content efficiently.

## Team

- **Timothée**: Focused on the parsing for the configuration aspect of the server.
- **Alfa**: Handled request and response processing.

## Features

- HTTP server compliant with C++ 98 standards.
- Handles GET, POST, DELETE methods.
- Supports file uploads and multiport listening.
- Configuration file for server settings.
- Non-blocking I/O operations with `poll`.
- Default error pages.
- Compatible with popular web browsers.
- Stress-tested for robustness.

## Installation

1. Clone the repository: `https://git.devo.pw/alfa/webserv.git`
2. Compile the source with `make`.
3. Run the server with our basic confi file `make run` or `./webserv [configuration file]`.

## Configuration

- The configuration file format is inspired by NGINX.
- You can specify host, port, server names, error pages, client body size limit, and routing rules.

## Testing

- Compared against NGINX for HTTP 1.1 compliance.
- Includes various functional and stress tests.

## Contributions

This project is part of an educational initiative. Contributions and feedback are welcome, adhering to the project's standards.

## Requirements

- C++ 98 compliant compiler.
- No external or Boost libraries allowed.
- For MacOS, use `fcntl(fd, F_SETFL, O_NONBLOCK)` for non-blocking mode.

## Installation and Usage

1. Clone the repository.
2. Use the provided Makefile to compile the source: `make`.
3. Run the server: `./webserv [configuration file]`.

## Configuration
