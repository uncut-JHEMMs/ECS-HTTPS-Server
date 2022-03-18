#pragma once

#define XMLSEC_CRYPTO_OPENSSL

//#include <xmlsec/xmldsig.h>
#include <memory>
#include "../Logging.h"
//#include <libxml/tree.h>
//#include <libxml/xmlmemory.h>
//#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/security.h>
#endif  //XMLSEC_NO_XSLT 

//#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/crypto.h>

class DocumentSigning{
private:
  std::shared_ptr<char[]> private_key;
  Logging& log = Logging::createLog();
  
  DocumentSigning(std::shared_ptr<char[]> p_key){
    this->private_key = p_key;
    p_key = nullptr;
  }

  DocumentSigning(DocumentSigning&) = delete;
  DocumentSigning(DocumentSigning&&) = delete;

public:
  
  DocumentSigning& operator=(DocumentSigning&& signer){
    this->private_key = signer.private_key;
    signer.private_key = nullptr;

    return *this;
  }

  DocumentSigning& operator=(DocumentSigning&) = delete;
  
  static DocumentSigning& createDocumentSigner(const std::shared_ptr<char[]> p_key){
    static DocumentSigning signer(p_key);

#ifndef XMLSEC_NO_XSLT
    xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif
    
    /* Init libxml and libxslt libraries */
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
    xmlIndentTreeOutput = 1; 
#endif
    
    #ifndef XMLSEC_NO_XSLT
    /* disable everything */
    xsltSecPrefs = xsltNewSecurityPrefs(); 
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_FILE, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_FILE, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_NETWORK, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_NETWORK, xsltSecurityForbid);
    xsltSetDefaultSecurityPrefs(xsltSecPrefs); 
#endif
    
    if(xmlSecInit() < 0){
      //log.log("Error: xmlsec initialization failed");
    }

    #ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
    if(xmlSecCryptoDLLoadLibrary(NULL) < 0) {
      log.log("Error: unable to load default xmlsec-crypto library. Make sure\nthat you have it installed and check shared libraries path\n (LD_LIBRARY_PATH and/or LTDL_LIBRARY_PATH) environment variables.\n");
    }
#endif

    if(xmlSecCryptoAppInit(NULL) < 0) {
      //log.log("Error: crypto initialization failed.\n");
    }

    if(xmlSecCryptoInit() < 0) {

    }
    
    return signer;
  }
  
  bool sign_file(const std::shared_ptr<char[]>& filename, int data_size){

    auto clean_up = [] (xmlSecDSigCtxPtr dsigCtx, xmlDocPtr doc){
		      xmlSecDSigCtxDestroy(dsigCtx);
		      xmlFreeDoc(doc);
		    };
    
    //load the file data to sign
    xmlDocPtr doc = xmlParseFile(filename.get());//xmlParseMemory(filename.get(), data_size);
    
    if(doc == nullptr){
      log.log("Problem signing document. Doc returned null.");
      xmlFreeDoc(doc);
      return false;
    }

    //find the start node
    xmlNodePtr node = xmlSecFindNode(xmlDocGetRootElement(doc), xmlSecNodeSignature, xmlSecDSigNs);

    if(node == nullptr){
      log.log("Problem signing document. Couldn't find root node.");
      xmlFreeDoc(doc);
      return false;
    }

    //Create the signature context
    xmlSecDSigCtxPtr dsigCtx = xmlSecDSigCtxCreate(nullptr);

    if(dsigCtx == nullptr){
      log.log("Problem signing document. Failed to create signature context.");
      clean_up(dsigCtx, doc);
      return false;
    }

    //load private key
    dsigCtx->signKey = xmlSecCryptoAppKeyLoad(private_key.get(), xmlSecKeyDataFormatPem, nullptr, nullptr, nullptr);
    if(dsigCtx->signKey == nullptr){
      log.log("Problem signing document. Failed to load private pem key.");
      clean_up(dsigCtx, doc);
      return false;
    }

    //set key name to the file name
    
    if(xmlSecKeySetName(dsigCtx->signKey, (const unsigned char*) private_key.get()) < 0){
      log.log("Problem signing document. Failed to set key name.");
      clean_up(dsigCtx, doc);
      return false;
    }

    //sign the data
    if(xmlSecDSigCtxSign(dsigCtx, node) < 0){
      log.log("Problem signing document. Signature failed.");
      clean_up(dsigCtx, doc);
      return false;
    }

    xmlDocDump(stdout, doc);

    //clean up
    clean_up(dsigCtx, doc);
    
    return true;
  }
};
