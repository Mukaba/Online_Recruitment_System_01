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
 | Filename: check_for_duplicates.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to check for duplicate companies and          |
 | contacts.  This script must be provided with a job_id that corresponds to    |
 | the pendingEmployer information.                                             |
 +------------------------------------------------------------------------------+

*/

$duplicates_found = 0;

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

$checkCompanyName = $pendingEmployer->company_name[0] . $pendingEmployer->company_name[1] . $pendingEmployer->company_name[2];
$checkContactLastName = $pendingEmployer->contact_last_name[0] . $pendingEmployer->contact_last_name[1] . $pendingEmployer->contact_last_name[2];

// First get all of the company/contact pairs that match the approved contact and company.

$sql = ("
	SELECT DISTINCT ec.employer_id, ec.company_name, ce.contact_id, c.first_name, c.last_name
	FROM employer_company AS ec, contact_employer AS ce, contact AS c
	WHERE ec.employer_id=ce.employer_id
	AND c.contact_id=ce.contact_id
	AND ec.company_name LIKE '" . addslashes($checkCompanyName) . "%'
	AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
	AND c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
	AND ec.company_display
	ORDER BY ec.company_name, c.last_name, c.first_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	// We have matching company/contact pairs in the database.

	while ($row = $result->FetchRow())
	{
		$similar_company_contacts[] = array(
			"employer_id" => $row["employer_id"],
			"company_name" => $row["company_name"],
			"contact_id" => $row["contact_id"],
			"contact_name" => $row["first_name"] . ' ' . $row["last_name"]
			);

		// Keep track of the company and contact ids we've gotten so far so we don't reuse them further down.

		$employer_ids .= ("'" . $row["employer_id"] . "',");
		$contact_ids .= ("'" . $row["contact_id"] . "',");
	}

	$duplicates_found = 1;
}

if (!$employer_ids)
{
	$employer_ids = ("''");
}
else
{
	$employer_ids = substr($employer_ids, 0, -1);
}

if (!$contact_ids)
{
	$contact_ids = ("''");
}
else
{
	$contact_ids = substr($contact_ids, 0 ,-1);
}

// Next get all of the companies that match, but that don't have a matching contact.

$sql = ("
	SELECT DISTINCT employer_id, company_name
	FROM employer_company
	WHERE company_name LIKE '" . addslashes($checkCompanyName) . "%'
	AND employer_id NOT IN (" . $employer_ids . ")
	AND company_display
	ORDER BY company_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	// We have matching companies in the database.

	while ($row = $result->FetchRow())
	{
		$similar_companies[] = array(
			"employer_id" => $row["employer_id"],
			"company_name" => $row["company_name"]
			);
	}

	$duplicates_found = 1;
}

$sql = ("
	SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, c.contact_id, ec.employer_id, ec.company_name
	FROM contact AS c, employer_company AS ec, contact_employer AS ce
	WHERE c.last_name LIKE '" . addslashes($checkContactLastName) . "%'
	AND c.first_name LIKE '" . addslashes($pendingEmployer->contact_first_name[0]) . "%'
	AND c.type='employer'
	AND ce.contact_id=c.contact_id
	AND ec.employer_id=ce.employer_id
	AND c.contact_id NOT IN (" . $contact_ids . ")
	ORDER BY c.last_name, c.first_name, ec.company_name
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
	// We have matching contacts in the database.
	
	while ($row = $result->FetchRow())
	{
		$similar_contacts[] = array(
			"contact_id" => $row["contact_id"],
			"contact_name" => $row["name"],
			"employer_id" => $row["employer_id"],
			"company_name" => $row["company_name"]
			);
	}

	$duplicates_found = 1;
}

if ($duplicates_found)
{
	include('job_descriptions/pending_jobs/duplicates_found.inc');
}
else
{
	$choice = 'newCompany_newContact';
	include('job_descriptions/pending_jobs/approve_job/method_for_adding.inc');
}

?>

