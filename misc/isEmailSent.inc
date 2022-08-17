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
 | Filename: isEmailSent.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: isEmailSent.inc

// functions
// boolean isEmployerEmailSent(int $job_id)
// boolean isSecretaryEmailSent(int $job_id)
// boolean isNewsgroupEmailSent(int $job_id)


function isEmployerEmailSent($job_id) { return isEmailSent($job_id, "employer"); }
function isSecretaryEmailSent($job_id) { return isEmailSent($job_id, "secretary"); }
function isNewsgroupEmailSent($job_id) { return isEmailSent($job_id, "newsgroup"); }


function isEmailSent($job_id, $type=false)
{
	switch ($type)
	{
		case "secretary": $clause = "emailed_secretary=1"; break;
		case "newsgroup":	$clause = "emailed_newsgroup=1";	break;
		
		case "employer":
		default: $clause = "emailed_employer=1";
	}
	
	$result = $GLOBALS['dbh']->Execute("
		SELECT	job_id
		FROM		interview
		WHERE		job_id='" . addslashes($job_id) . "' AND " . $clause);
					
	if (!$result) { return false; }
	$isEmailSent = (($row = $result->FetchRow()) && ($row["job_id"] == $job_id));
	$result->Close();
	
	return $isEmailSent;
}

?>