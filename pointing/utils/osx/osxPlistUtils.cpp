/* -*- mode: c++ -*-
 *
 * pointing/utils/osx/osxPlistUtils.cpp --
 *
 * Initial software
 * Authors: Nicolas Roussel
 * Copyright © Inria
 *
 * http://libpointing.org/
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#include <pointing/utils/osx/osxPlistUtils.h>

#include <iostream>

namespace pointing {

  CFPropertyListRef
  getPropertyListFromXML(const char *xml) {
    CFDataRef cfdata = CFDataCreate(kCFAllocatorDefault, (const UInt8*)xml, strlen(xml)) ;
    CFPropertyListRef plist = CFPropertyListCreateWithData(kCFAllocatorDefault,
							   cfdata,
							   kCFPropertyListImmutable,
							   NULL, NULL) ;
    CFRelease(cfdata) ;
    return plist ;
  }

  CFPropertyListRef
  getPropertyListFromFile(const char *path) {
#if 0
    CFStringRef cfpath = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault,
							 path,
							 kCFStringEncodingISOLatin1,
							 kCFAllocatorNull) ;
    CFURLRef cfurl = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfpath, 
						   kCFURLPOSIXPathStyle, false) ;
    CFRelease(cfpath) ;
    CFDataRef xmldata = 0 ;
    SInt32 errorCode = 0 ;
    // FIXME: this function is now deprecated
    Boolean ok = CFURLCreateDataAndPropertiesFromResource(kCFAllocatorDefault,
							  cfurl,
							  &xmldata,
							  NULL,
							  NULL,
							  &errorCode) ;
    CFRelease(cfurl) ;
    if (!ok) {
      std::cerr << "getPropertyListFromFile: error " << errorCode << " when trying to create CFURL" << std::endl ;
      return 0 ;
    }

    CFPropertyListRef plist = CFPropertyListCreateWithData(kCFAllocatorDefault,
							   xmldata,
							   kCFPropertyListImmutable,
							   NULL, NULL) ;
    CFRelease(xmldata) ;
#else
    CFStringRef cfpath = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault,
							 path,
							 kCFStringEncodingISOLatin1,
							 kCFAllocatorNull) ;
    CFURLRef cfurl = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfpath, 
						   kCFURLPOSIXPathStyle, false) ;
    CFReadStreamRef stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, cfurl );

    CFRelease(cfurl) ;
    CFRelease(cfpath) ;

    if (CFReadStreamOpen(stream) == FALSE) {
        std::cerr << "CFReadStreamOpen: error" << std::endl ;
	CFRelease(stream) ;
        return 0 ;
    }
    
    CFErrorRef error = NULL ;
    CFPropertyListRef plist = CFPropertyListCreateWithStream(kCFAllocatorDefault,
							     stream, 0 /* streamLength */,
							     kCFPropertyListImmutable,
							     NULL /* *format */,
							     &error /* *error*/) ;
    // Handle the error
    if (error) {
    	/*
    	CFString errordesc = CFErrorCopyDescription(error);
	CFRelease(errordesc);
	*/
        std::cerr << "CFPropertyListCreateWithStream: error" << std::endl ;
	CFRelease(stream) ;
        return 0 ;
    }
    
    CFReadStreamClose(stream) ;
    CFRelease(stream) ;    
#endif

    return plist ;
  }

}
