# ECS-HTTPS-Server

Instructions -
	     create_DIRs.sh - creates the directories for all of the CA's.
	     create_CA.sh - Creates a CA using two command line arguments. First being a relative path to a location for the private key or used to specify a test. This path includes a key name. And second being a relative path to the location to store the certificate this is also needed when running tests. This also includes the certificate name.

	     create_CSR.sh - Takes two arguments. A relative path for a private key as well as the key name. The second a relative path for a csr including its name.

	     createCRL.sh - This creates a crl list. It takes two arguments and one is a relative path, including file name, to where to store the list. This can also be test followed by another argument with a relative path to a crl to test its validity.

	     createPFX.sh - Creates a pfx bundle. This takes three arguments. The first being a certificate, the second being a key name, and third being a pfx name. The limitation of this is it does not allow multiple certificates and keys. To test pass arguments test and a pfx name.
	     
	     signCSR.sh - Does as the name suggests. Takes three command line arguments. The first being the relative path to a csr or test for testing; the second argument the relative path to a signing certificate authority, and the third is a relative path to a signed certificate. This is also used for testing purposes and can be left blank when not testing. These last two are needed for testing as well.
	     
	     addRevoke.sh - takes one argument and it is a relative path to a certificate to revoke its permissions.
	     