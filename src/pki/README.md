
Instructions -

	     createCA_Dir.sh - creates data directories for server, document, and root CAs. These directories include keys, certificates, crl's, and pfx bundles.
	     create_CAs.sh - Creates CAs, crls, pfx bundles and csrs for each (root, server, document). 

	     createCSR.sh - This creates a csr and takes two arguments. The first being a key filename and the second a csr file name.
	     
	     addRevoke.sh - Takes two arguments and one is a relative path to a config file. The other is a relative path to a certificate to revoke its permissions.
	     