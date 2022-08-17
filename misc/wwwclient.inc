<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: wwwclient.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


class wwwclient
{
   var $browser;
   var $platform;

   function wwwclient()
   {
      global $HTTP_USER_AGENT;
      
      if (preg_match("/MSIE/", $HTTP_USER_AGENT))
      {
         $this->browser = IE;
      }
      elseif (preg_match("/Safari/", $HTTP_USER_AGENT))
      {
         $this->browser = SAFARI;
      }
      elseif (preg_match("/Lynx/", $HTTP_USER_AGENT))
      {
         $this->browser = LYNX;
      }
      elseif (preg_match("/Fire/", $HTTP_USER_AGENT))
      {
         $this->browser = FIREFOX;
      }
      elseif (preg_match("/Safari/", $HTTP_USER_AGENT))                                                                                        
      {                                                                                                                                              
          $this->browser = SAFARI;                                                                                                                  
      }                            
      elseif (preg_match("/Phoenix/", $HTTP_USER_AGENT))                                                                                        
      {                                                                                                                                              
         $this->browser = FIREFOX;                                                                                                                  
      }
      elseif (preg_match("/Mozilla\/4|5/", $HTTP_USER_AGENT))
      {      
         $this->browser = NETSCAPE;                
      }                   
      elseif (preg_match("/Gecko/", $HTTP_USER_AGENT))
      {
         $this->browser = GECKO;          
      }    
      else
      {
         $this->browser = UNKNOWN;
      }

      if (preg_match("/Mac/", $HTTP_USER_AGENT))
      {
         $this->platform = MAC;
      }
      elseif (preg_match("/Win/", $HTTP_USER_AGENT))
      {
         $this->platform = WIN;
      }
      elseif (preg_match("/Linux/", $HTTP_USER_AGENT))
      {
         $this->platform = LINUX;
      } 
      elseif (preg_match("/SunOS/", $HTTP_USER_AGENT))
      {
	$this->platform = UNIX;
      }
      else
      {
         $this->platform = UNKNOWN;
      }      
   }
}

?>
