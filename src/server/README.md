To start the server run the start script with a command line argument pointing to a server configuration file made with json, or set environment variables for the server to use. The names of the variables to be used in the configuration file and environment variables are:
   - port (optional)
   - max_threads (optional)
   - max_connections (optional)
   - connection_timeout (optional)
   - memory_limit (optional)
   - server_key (required)
   - server_ca (required)
   - server_cert (required)
   
Included is a config file with some default parameters.