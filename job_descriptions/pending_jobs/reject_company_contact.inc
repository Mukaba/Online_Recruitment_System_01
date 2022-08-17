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
 | Filename: reject_company_contact.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file is called when the user has confirmed that they       |
 | would like to reject the company and contact information that they have      |
 | received.  It deletes the company, contact, and job info from the pending    |
 | tables in the database, and then alerts the user of success.                 |
 +------------------------------------------------------------------------------+

*/

assert(isset($pendingEmployer));

global $REMOTE_ADDR;

// Save the IP address and some of the information of the company, contact, and job we are deleting

$sql = ("
	INSERT INTO rejected_employer_ips
	(date_rejected, IP_address, position_title, company_name, contact_first_name, contact_last_name)
	VALUES
	(
	CURRENT_DATE,
	'" . addslashes(trim($pendingEmployer->IP_added_from)) . "',
	'" . addslashes(trim($pendingEmployer->job_position_title)) . "',
	'" . addslashes(trim($pendingEmployer->company_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_first_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_last_name)) . "'
	)
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Delete contact information.

$sql = ("
	DELETE FROM pending_employer_contact
	WHERE contact_id='" . addslashes($pendingEmployer->contact_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Delete company information

$sql = ("
	DELETE FROM pending_employer_company
	WHERE employer_id='" . addslashes($pendingEmployer->employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Delete job information

$sql = ("
	DELETE FROM pending_job_description
	WHERE job_id='" . addslashes($pendingEmployer->job_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

notify("The company, contact, and job information have been successfully deleted from the database.");

?>
