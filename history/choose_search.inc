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
 | Filename: choose_search.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: choose the searching level (Simple or Advanced) based on what   |
 | the user has set in his preferences                                          |
 +------------------------------------------------------------------------------+

*/
if (isset ($searchmode))
{
	if (!strcmp($searchmode, "advanced")){
    		include ("history/advanced_search_form.inc");
  	}else{
    		include ("history/simple_search.inc");
  	}
}
else
{
	$sql = "select ci.search_use_advanced_hist from contact_internal as ci where ci.login_id=\"$login_id\" or ci.netlink_id=\"$login_id\" and ci.department_id=".$auth->department;
    $response = $GLOBALS['dbh']->Execute($sql);

  	if (!$response->RecordCount())
  	{
    		// I dunno who this is, but he's been here long enough to belong here. Default to simple search
    		include ("history/simple_search.inc");
  	}
  	else
  	{
    		$row = $response->FetchRow();
    		$search_use_advanced = $row['search_use_advanced_hist'];
    		if ($advance=='TRUE'){
        		$search_use_advanced=1;
    		}
    		if ($search_use_advanced == 1)
    		{
      			include ("history/advanced_search_form.inc"); // Complicated search
    		}
    		else
    		{
      			$select = "view_history";
      			include ("history/simple_search.inc");
    		}
  	}
}
?>
