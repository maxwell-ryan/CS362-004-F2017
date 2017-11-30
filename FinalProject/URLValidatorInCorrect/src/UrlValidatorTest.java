/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }
  
   
   private static final String[] MANUAL_TEST_URLS = new String[] {
			"http://foo.com",					//benign test case
			"http://foo3.com/123/hellow0rld",	//numbers in hostname, path
			"http://foo.com/blah_bla*h/",		//'safe' characters in path
			"http://foo.com/blah_bla*h(paren)_(paren)/",		//more 'safe' characters in path
			"http://foo.com/0123$-_.+!/*|(),/",			//safe characters - fail
			"http://foo.com/\"%<>\\^`{|}",		// unsafe characters in path
			"HTTP://WWW.FOO.COM",				//all caps
			" http://foo.com",					//add space before scheme
			"hello://foo.com",					// random scheme name
			"ht!tp://foo.com",					//non alphanumeric in scheme
			"http://f oo.com",					//space within URL
			"http://foo.com ",					//trailing space
			"http://223.255.255.254",			//DNS format
			"http://foo.com/?q=testquery",		//add simple query section unexpected FAIL
			"http://foo.bar/?q=Test%20URLencoded%20stuff",	//URLencoded - fail
			// very long and complicated path (fail):			
			"http://foo:bar@w1.superman.com/very/long/path.html?p1=v1&p2=v2#more-details",
			"http://foo.com/a#b#c#",				// fragments
			"http://foo.com/a#b c#",				// space in fragements
			"ftp://ftp.rfc-editor.org/",			//ftp 
			"gopher://floodgap.com"				//gopher scheme
	};
   
   public void testManualTest()
   {
	   System.out.println("Manual tests and results:");
	   System.out.println("----------------------------------------");
	   for(int i = 0; i < MANUAL_TEST_URLS.length; i++)
	   {
		   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		   System.out.println(MANUAL_TEST_URLS[i]);
		   System.out.println(urlVal.isValid(MANUAL_TEST_URLS[i]));
		   System.out.println();
	   }
	   System.out.println("Manual testing FINISHED.");
	   System.out.println("----------------------------------------");
   }
   
   
   /*url is composed for following components:
   1) application layer protocol (http, ftp...)
   2) hostname (google, espn, www.wallstreetjournal, engr.oregonstate, 156.34.28.112)
   3) top level domain (aka. COM, EDU, NET)
   4) path (/word, /word/anotherword, /chars/chars/chars)
   */
   
   public void testYourFirstPartition() {
	   
	   System.out.println("\nTest - Partition 1 - URL protocol");
	   System.out.println("----------------------------------------\n");
	   
	   //track if any at least one failure found in entire partition set test
	   boolean protocolOverallResult = true;
	   
	   //construct array of 'good' protocol partition inputs
	   ResultPair[] validProtocolTestInputs = {
			   new ResultPair("http", true),
			   new ResultPair("ftp", true),
			   new ResultPair("https", true),
			   new ResultPair("ftps", true),
			   new ResultPair("telnet", true)
	   };
	   
	   //construct array of 'bad' protocol partition inputs
	   ResultPair[] invalidProtocolTestInputs = {
			   new ResultPair("fttps", false),
			   new ResultPair("htp", false),
			   new ResultPair("htt", false),
			   new ResultPair("httpp", false),
			   new ResultPair("htttp", false),
			   new ResultPair("ttp", false),
			   new ResultPair("", false),
			   new ResultPair(" ", false),
	   };
	   
	   String[] validProtocols = {"http", "https", "ftp", "ftps", "telnet"};
	   
	   boolean protocolTestResult;
	   UrlValidator testURLValidator = new UrlValidator(validProtocols);
	   
	   //iterate over all good partition inputs and ensure that PUT agrees
	   for (int x = 0; x < validProtocolTestInputs.length; x++) {
		   ResultPair currentResultPair = validProtocolTestInputs[x];
		  
		   protocolTestResult = testURLValidator.isValidScheme(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, protocolTestResult, currentResultPair.valid);
		   if (protocolTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Protocol " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Protocol " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   protocolOverallResult = false;
		   }
	   }
	   
	   //iterate over all bad partition inputs and ensure that PUT agrees
	   for (int x = 0; x < invalidProtocolTestInputs.length; x++) {
		   ResultPair currentResultPair = invalidProtocolTestInputs[x];
		  
		   protocolTestResult = testURLValidator.isValidScheme(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, protocolTestResult, currentResultPair.valid);
		   if (protocolTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Protocol " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Protocol " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   protocolOverallResult = false;
		   }
	   }
	   
	   System.out.println("\nPartition 1 testing FINISHED.");
	   if (protocolOverallResult == true) {
		   System.out.println("Partition 1 testing PASSED"); 
	   } else {
		   System.out.println("Partition 1 testing DID NOT PASS, FAILURE(s) were found"); 
	   }
	   System.out.println("----------------------------------------");
   }
   
   public void testYourSecondPartition() {
	   
	   System.out.println("\nTest - Partition 2 - authority");
	   System.out.println("----------------------------------------\n");
	   
	   //track if any at least one failure found in entire partition set test
	   boolean authorityOverallResult = true;
	   
	   //construct array of 'good' authority partition inputs
	   ResultPair[] validAuthorityTestInputs = {
			   new ResultPair("www.espn.com", true),
			   new ResultPair("espn.com", true),
			   new ResultPair("google.com", true),
			   new ResultPair("oregonstate.edu", true),
			   new ResultPair("microsoft.net", true),
			   new ResultPair("theverge.com:80", true),
			   new ResultPair("151.101.1.52", true)
	   };
	   
	   //construct array of 'bad' authority partition inputs (blank authority considered invalid)
	   ResultPair[] invalidAuthorityTestInputs = {
			   new ResultPair(".com", false),
			   new ResultPair("google..com", false),
			   new ResultPair("..oregonstate.edu", false),
			   new ResultPair("et", false),
			   new ResultPair("theverge.com:::80", false),
			   new ResultPair("151.101.1.52.6", false),
			   new ResultPair("151.101.1", false),
			   new ResultPair("151.260.1.52", false),
			   new ResultPair(null, false)
	   };
	   
	   boolean authorityTestResult;
	   UrlValidator testURLValidator = new UrlValidator();
	   
	   //iterate over all good partition inputs and ensure that PUT agrees
	   for (int x = 0; x < validAuthorityTestInputs.length; x++) {
		   ResultPair currentResultPair = validAuthorityTestInputs[x];
		  
		   authorityTestResult = testURLValidator.isValidAuthority(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, authorityTestResult, currentResultPair.valid);
		   if (authorityTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Authority " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("Failure: Authority " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   authorityOverallResult = false;
		   }
	   }
	   
	   //iterate over all bad partition inputs and ensure that PUT agrees
	   for (int x = 0; x < invalidAuthorityTestInputs.length; x++) {
		   ResultPair currentResultPair = invalidAuthorityTestInputs[x];
		  
		   authorityTestResult = testURLValidator.isValidAuthority(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, authorityTestResult, currentResultPair.valid);
		   if (authorityTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Authority " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("Failure: Authority " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   authorityOverallResult = false;
		   }
	   }
	   
	   System.out.println("\nPartition 2 testing FINISHED.");
	   if (authorityOverallResult == true) {
		   System.out.println("Partition 2 testing PASSED"); 
	   } else {
		   System.out.println("Partition 2 testing DID NOT PASS, FAILURE(s) were found"); 
	   }
	   System.out.println("----------------------------------------");
   }
   
   public void testYourThirdPartition() {
	   
	   System.out.println("\nTest - Partition 3 - port number");
	   System.out.println("----------------------------------------\n");
	   
	   //track if any at least one failure found in entire partition set test
	   boolean portOverallResult = true;
	   
	   //construct array of 'good' authority partition inputs
	   ResultPair[] validPortTestInputs = {
			   new ResultPair("espn.com:80", true),
			   new ResultPair("espn.com:65535", true),
			   new ResultPair("espn.com:10000", true),
			   new ResultPair("espn.com:1234", true),
			   new ResultPair("espn.com:9999", true),
			   new ResultPair("espn.com:0000", true),
			   new ResultPair("espn.com:123", true),
			   new ResultPair("espn.com:38", true),
			   new ResultPair("espn.com:0", true)
	   };
	   
	   //construct array of 'bad' authority partition inputs (blank authority considered invalid)
	   ResultPair[] invalidPortTestInputs = {
			   new ResultPair(null, false),
			   new ResultPair("espn.com:", false),
			   new ResultPair("", false),
			   new ResultPair("espn.com:65536", false),
			   new ResultPair("espn.com:111111", false),
			   new ResultPair("espn.com:-1", false)
	   };
	   
	   boolean portTestResult;
	   UrlValidator testURLValidator = new UrlValidator();
	   
	   //iterate over all good partition inputs and ensure that PUT agrees
	   for (int x = 0; x < validPortTestInputs.length; x++) {
		   ResultPair currentResultPair = validPortTestInputs[x];
		  
		   portTestResult = testURLValidator.isValidAuthority(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, portTestResult, currentResultPair.valid);
		   if (portTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Port " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("Failure: Port " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   portOverallResult = false;
		   }
	   }
	   
	   //iterate over all bad partition inputs and ensure that PUT agrees
	   for (int x = 0; x < invalidPortTestInputs.length; x++) {
		   ResultPair currentResultPair = invalidPortTestInputs[x];
		  
		   portTestResult = testURLValidator.isValidAuthority(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, portTestResult, currentResultPair.valid);
		   if (portTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Port " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("Failure: Port " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   portOverallResult = false;
		   }
	   }
	   
	   System.out.println("\nPartition 3 testing FINISHED.");
	   if (portOverallResult == true) {
		   System.out.println("Partition 3 testing PASSED"); 
	   } else {
		   System.out.println("Partition 3 testing DID NOT PASS, FAILURE(s) were found"); 
	   }
	   System.out.println("----------------------------------------");
   }
   
   public void testYourFourPartition() {
	   
	   System.out.println("\nTest - Partition 4 - Path");
	   System.out.println("----------------------------------------\n");
	   
	   //track if any at least one failure found in entire partition set test
	   boolean pathOverallResult = true;
	   
	   //construct array of 'good' path partition inputs
	   ResultPair[] validPathTestInputs = {
			   new ResultPair("/test/the/path", true),
			   new ResultPair("/test/the/path/", true),
			   new ResultPair("/", true),
			   new ResultPair("/test", true),
			   new ResultPair("/test/", true),
			   new ResultPair("/12/123/1234", true),
			   new ResultPair("/@/", true)
	   };
	   
	   //construct array of 'bad' path partition inputs
	   ResultPair[] invalidPathTestInputs = {
			   new ResultPair("/test\the/path", false),
			   new ResultPair("\test/the/path/", false),
			   new ResultPair("//", false),
			   new ResultPair("t/est", false),
			   new ResultPair("/tes//t", false),
			   new ResultPair("121231234", false),
			   new ResultPair(null, false)
	   };
	   
	   boolean pathTestResult;
	   UrlValidator testURLValidator = new UrlValidator();
	   
	   //iterate over all good partition inputs and ensure that PUT agrees
	   for (int x = 0; x < validPathTestInputs.length; x++) {
		   ResultPair currentResultPair = validPathTestInputs[x];
		  
		   pathTestResult = testURLValidator.isValidPath(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, pathTestResult, currentResultPair.valid);
		   if (pathTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Path " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Path " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   pathOverallResult = false;
		   }
	   }
	   
	   //iterate over all bad partition inputs and ensure that PUT agrees
	   for (int x = 0; x < invalidPathTestInputs.length; x++) {
		   ResultPair currentResultPair = invalidPathTestInputs[x];
		  
		   pathTestResult = testURLValidator.isValidPath(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, pathTestResult, currentResultPair.valid);
		   if (pathTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Path " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Path " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   pathOverallResult = false;
		   }
	   }
	   
	   System.out.println("\nPartition 4 testing FINISHED.");
	   if (pathOverallResult == true) {
		   System.out.println("Partition 4 testing PASSED"); 
	   } else {
		   System.out.println("Partition 4 testing DID NOT PASS, FAILURE(s) were found"); 
	   }
	   System.out.println("----------------------------------------");
   }
   
   
   public void testYourFifthPartition() {
	   
	   System.out.println("\nTest - Partition 5 - Query");
	   System.out.println("----------------------------------------\n");
	   
	   //track if any at least one failure found in entire partition set test
	   boolean queryOverallResult = true;
	   
	   //construct array of 'good' path partition inputs
	   ResultPair[] validQueryTestInputs = {
			   new ResultPair("?variable=true", true),
			   new ResultPair("?variable=true&secondVar=false", true),
			   new ResultPair("?", true),
			   new ResultPair("?var=true&secondVar=false&thirdVar=int", true),
			   new ResultPair("", true),
	   };
	   
	   //construct array of 'bad' path partition inputs
	   ResultPair[] invalidQueryTestInputs = {
			   new ResultPair("??variable=true", false),
			   new ResultPair("?var==true", false),
	   };
	   
	   boolean queryTestResult;
	   UrlValidator testURLValidator = new UrlValidator();
	   
	   //iterate over all good partition inputs and ensure that PUT agrees
	   for (int x = 0; x < validQueryTestInputs.length; x++) {
		   ResultPair currentResultPair = validQueryTestInputs[x];
		  
		   queryTestResult = testURLValidator.isValidQuery(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, queryTestResult, currentResultPair.valid);
		   if (queryTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Path " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Path " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   queryOverallResult = false;
		   }
	   }
	   
	   //iterate over all bad partition inputs and ensure that PUT agrees
	   for (int x = 0; x < invalidQueryTestInputs.length; x++) {
		   ResultPair currentResultPair = invalidQueryTestInputs[x];
		  
		   queryTestResult = testURLValidator.isValidQuery(currentResultPair.item);
		   
		   //assertEquals(currentResultPair.item, queryTestResult, currentResultPair.valid);
		   if (queryTestResult == currentResultPair.valid) {
			   System.out.println("PASS: Path " + currentResultPair.item + " correctly determined to be: " + String.valueOf(currentResultPair.valid));
		   } else {
			   System.out.println("FAILURE: Path " + currentResultPair.item + " incorrectly determined to be: " + String.valueOf(!currentResultPair.valid));
			   queryOverallResult = false;
		   }
	   }
	   
	   System.out.println("\nPartition 5 testing FINISHED.");
	   if (queryOverallResult == true) {
		   System.out.println("Partition 5 testing PASSED"); 
	   } else {
		   System.out.println("Partition 5 testing DID NOT PASS, FAILURE(s) were found"); 
	   }
	   System.out.println("----------------------------------------");
   }
   
   //Create Result pair for each of the url components

   /*------------------------------------------------*/
   //1)construct array of 'good' protocol partition inputs
   ResultPair[] validProtocolTestInputs = {
           new ResultPair("http", true),
           new ResultPair("ftp", true),
           new ResultPair("https", true),
           new ResultPair("ftps", true),
           new ResultPair("telnet", true),

           new ResultPair("fttps", false),
           new ResultPair("htp", false),
           new ResultPair("htt", false),
           new ResultPair("httpp", false),
           new ResultPair("htttp", false),
           new ResultPair("ttp", false),
           new ResultPair("", false),
           new ResultPair(" ", false),

   };

   /*------------------------------------------------*/
   //2)construct array of 'good' authority partition inputs with port numbers
   ResultPair[] validHostnameTestInputs = {
           new ResultPair("www.espn.com", true),
           new ResultPair("espn.com", true),
           new ResultPair("google.com", true),
           new ResultPair("oregonstate.edu", true),
           new ResultPair("microsoft.net", true),
           new ResultPair("theverge.com:80", true),
           new ResultPair("151.101.1.52", true),

           new ResultPair(".com", false),
           new ResultPair("google..com", false),
           new ResultPair("..oregonstate.edu", false),
           new ResultPair("et", false),
           new ResultPair("theverge.com:::80", false),
           new ResultPair("151.101.1.52.6", false),
           new ResultPair("151.101.1", false),
           new ResultPair("151.260.1.52", false),
           new ResultPair(null, false),
           new ResultPair("localhost",true),
           new ResultPair("localdomain",true),
           new ResultPair("www.google.na", true),
           new ResultPair("www.google.us", true),
           new ResultPair("www.google.uk", true),
           new ResultPair("google.na", true),
           new ResultPair("google.us", true),
           new ResultPair("google.uk", true),
           new ResultPair("www.google.com", true),



           new ResultPair("espn.com:80", true),
           new ResultPair("espn.com:65535", true),
           new ResultPair("espn.com:10000", true),
           new ResultPair("espn.com:1234", true),
           new ResultPair("espn.com:9999", true),
           new ResultPair("espn.com:0000", true),
           new ResultPair("espn.com:123", true),
           new ResultPair("espn.com:38", true),
           new ResultPair("espn.com:0", true),

           new ResultPair(null, false),
           new ResultPair("espn.com:", false),
           new ResultPair("", false),
           new ResultPair("espn.com:65536", false),
           new ResultPair("espn.com:111111", false),
           new ResultPair("espn.com:-1", false),

   };

   /*------------------------------------------------*/
   //3)
   //construct array of 'good' path partition inputs
   ResultPair[] validPathTestInputs = {
           new ResultPair("/test/the/path", true),
           new ResultPair("/test/the/path/", true),
           new ResultPair("/", true),
           new ResultPair("/test", true),
           new ResultPair("/test/", true),
           new ResultPair("/12/123/1234", true),
           new ResultPair("/@/", true),

           new ResultPair("/test\the/path", false),
           new ResultPair("\test/the/path/", false),
           new ResultPair("//", false),
           new ResultPair("t/est", false),
           new ResultPair("/tes//t", false),
           new ResultPair("121231234", false),
           new ResultPair(null, false)
   };


   //4)
   //contruct array of 'good' query partition inputs
   ResultPair[] querys = {
           new ResultPair("/?variable=true", true),
           new ResultPair("/?variable=true&secondVar=false", true),
           new ResultPair("/?", true),
           new ResultPair("/?var=true&secondVar=false&thirdVar=int", true),
           new ResultPair("/", true),
           new ResultPair("/??variable=true", false),
           new ResultPair("/?var==true", false)
   };

   public void testIsValid(){
       UrlValidator urlVal = new UrlValidator(null, null, (UrlValidator.ALLOW_ALL_SCHEMES + UrlValidator.ALLOW_LOCAL_URLS));

       for(int i = 0; i < validProtocolTestInputs.length; i++) {
           for(int j = 0; j < validHostnameTestInputs.length; j++) {
               for(int k = 0; k < validPathTestInputs.length; k++) {

                       boolean result = validProtocolTestInputs[i].valid &&
                               validHostnameTestInputs[j].valid &&
                               validPathTestInputs[k].valid;

                       String all = new StringBuilder(255).append(validProtocolTestInputs[i].item)
                               .append("://")
                               .append(validHostnameTestInputs[j].item)
                               .append(validPathTestInputs[k].item)
                               .toString();
                       //System.out.println(all);

                       if(urlVal.isValid(all) !=result) {
                           System.out.println("Failed: " + all);
                           System.out.println("  expected: " + result);
                       }
                       else {
                           //System.out.println("SUCESS: " + all);
                           assertEquals(result, urlVal.isValid(all));
                       }

               } //end path
          } //end hostname
      } //end protocol
   } //end is valid

}