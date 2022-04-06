Manual -

	     createCA_Dir.sh - creates data directories for server, document, and root CAs. These directories include keys, certificates, crl's, and pfx bundles.
	     
	     createCAs.cpp - Creates CAs from a json file that contains details about the certificates being created. I.e. their common name, state, etc. It creates three certificates. Server, root and document ca's.

	     Certificate_Configuration.h - This class reads a json file to gather all the certificate configuration data by using the keys listed below. This class is then submitted to the Certificate class in Certificate.h.

	     Certificate.h - Provides all the functionality for creating writing to a file certificates and their associated private keys. Used in association with the Certificate_Configuration class to get the certificates configurations.
	     
	     createCSR.sh - This creates a csr and takes two arguments. The first being a key filename and the second a csr file name.
	     
	     addRevoke.sh - Takes two arguments and one is a relative path to a config file. The other is a relative path to a certificate to revoke its permissions.
	     startCreateCAs.sh - compiles and runs the certificate authority program. This script has two flags. One being -h for help and the second -i. -i takes as an argument a json file containing certificate configuration info. The parameters are listed below, but the configuration file should have these 3 sections. RCA, SCA, DCA. Followed by their configurations.

	     setup_pki.sh - This will run the previous script as well as the others to establish the directory hierarchies and create CRL's (certificate revokation lists), index files, and serial number files.

Certificate Configuration Files:
	    Each certificate configuration has the following keys:
	    * common_name (string)
	    * country (string)
	    * city (string)
	    * organization (string)
	    * organizational_unit (string)
	    * cert_name (string)
	    * days (unsigned int)
	    * bits (unsigned int)
	    * serial (unsigned int)
	    * keyFileName (string)
	    * certFileName (string)