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
 | Filename: edit_contact.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file decides whether to load up the contact and employer   |
 | information form, or to save the inputs gathered from that form.  It also    |
 | validates any of the inputs.                                                 |
 +------------------------------------------------------------------------------+

*/

if ($continue == '' || $java_reload)
{
	/*
	 No form has been submitted yet, or we're dealing with resubmission from the java script, 
	 so give them the form to fill out.
	*/
	
	if (!isset($enable_save))
	{
		$enable_save = 'true';
	}
	
	if ($java_reload)
	{

		/*
		 If the form was submitted using javascript, save the values entered in.
		*/
		$job->set_value_3($company_name, $company_description, $company_website, $employer_id, $department_name,
        	$department_description, $department_website, $department_id, $contact_name, $contact_phone, $contact_cellphone, $contact_email,
        	$contact_fax, $contact_id);
	}
	if ($unset_company)
	{
		unset($job->company_name);
		unset($job->company_website);
		unset($job->company_description);
		
		/*
		 Just added code to unset lines below.  If there are new problems, they're probably right here.
		*/

		unset($job->department_name);
		unset($job->department_website);
		unset($job->department_description);
		unset($job->contact_name);
		unset($job->contact_phone);
        unset($job->contact_cellphone);
		unset($job->contact_email);
		unset($job->contact_fax);
	}
	if ($unset_department)
	{
		unset($job->department_name);
		unset($job->department_website);
		unset($job->department_description);
		
		/*
		 Just added code to unset lines below.  If there are new problems, they're probably right here.
		*/

		unset($job->contact_name);
		unset($job->contact_phone);
        unset($job->contact_cellphone);
		unset($job->contact_email);
		unset($job->contact_fax);
	}
	if ($unset_contact)
	{
		unset($job->contact_name);
		unset($job->contact_phone);
        unset($job->contact_cellphone);
		unset($job->contact_email);
		unset($job->contact_fax);
	}

	include('job_descriptions/edit_job/office/edit_contact_form.inc');
}

elseif ($continue == 'add_new_entity')
{
	/*
	 The user has chosen to add a new company, division, or contact.
	 Load the form to do this.
	*/
	
	include('job_descriptions/edit_job/office/add_new_entity.inc');
}

elseif ($continue == 'save_new_entity')  
{        
    /*
     We've just received some company, division, or contact information.
     Insert this into the appropriate database, retrieve the id number assigned
     to this information, and then reload add_job_form_a.inc
     */

    $error = 0;

    if ($add_this_object == 'employer')
    {

        /*
         Make sure we have a company name.
         */

        if (trim($company_name) == '')
        {
            $error = 1;
            error("You must specify a name for the new company.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif (!$add_confirmed)
        {
            // Check for similar company names in the database.
            $check_name = $company_name[0] . $company_name[1] . $company_name[2];

            $sql = ("
                    SELECT DISTINCT company_name
                    FROM employer_company
                    WHERE company_name LIKE '" . $check_name . "%'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;

                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ($row["company_name"]);
                    $msg .= (" is an exisiting company in the database with a name similar to");
                    $msg .= (" the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $company_names_array[] = $row["company_name"];
                    }
                    for ($i = 0; $i < (sizeof($company_names_array) - 1); $i++)
                    {
                        $msg .= ($company_names_array[$i] . ", ");
                    }
                    $msg .= ("and " . $company_names_array[$i] . " ");
                    $msg .= ("are companies with similar names to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
                }

                promptAddCompany($msg, $company_name, $company_website, $company_description, $job_id);
            }
        }

        if ($error == 0)
        {
            $sql = ("
                    INSERT INTO employer_company
                    (company_name, website, company_description, entered_by, entered_on)
                    VALUES
                    (
                     '" . addslashes(removeSlashes(trim($company_name))) . "',
                     '" . addslashes(removeSlashes(trim(normalizeWebsite($company_website)))) . "',
                     '" . addslashes(removeSlashes(trim($company_description))) . "',
                     '" . addslashes($auth->login) . "',
                     CURRENT_DATE
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS employer_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            // Update the status for this company for each department at our organization.  
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $dept_result = $GLOBALS['dbh']->Execute($sql);
            while ($dept_row = $dept_result->FetchRow())
            {
                updateCompanyStatus($row["employer_id"],$dept_row["department_id"]);
            }

            $job->employer_id = $row["employer_id"];

            /*
             Unset the website, and description for the job object, as well as any division
             and contact variables that have been set so that edit_contact_form pulls the values 
             out of the database for the newly created company.
             */

            unset($job->company_website);
            unset($job->company_description);
            unset($job->department_id);
            unset($job->department_website);
            unset($job->department_description);
            unset($job->contact_id);
            unset($job->contact_phone);
            unset($job->contact_cellphone);
            unset($job->contact_fax);
            unset($job->contact_email);

            $order = $company_name[0];
            include('job_descriptions/edit_job/office/edit_contact_form.inc');
        }
    }
    if ($add_this_object == 'department')
    {

        /*
         Make sure we have a division name.
         */

        if (trim($department_name) == '')
        {
            $error = 1;
            error("You must specify a name for the new division.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif (!$add_confirmed)
        {

            /*
             Check in the database for any divisions at this company with similar names, and ask the user if they
             sure they want to add THIS division.
             */

            $check_name = $department_name[0] . $department_name[1] . $department_name[2];

            $sql = ("
                    SELECT DISTINCT a.department_name, b.company_name
                    FROM employer_department AS a, employer_company AS b
                    WHERE a.department_name LIKE '" . $check_name . "%'
                    AND a.employer_id='" . $department_company_id . "'
                    AND b.employer_id='" . $department_company_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ($row["department_name"]);
                    $msg .= (" is an exisiting division at " . $row["company_name"] . " already in the database ");
                    $msg .= (" with a name similar to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this division to the database?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $department_names_array[] = $row["department_name"];
                    }
                    for ($i = 0; $i < (sizeof($department_names_array) - 1); $i++)
                    {
                        $msg .= ($department_names_array[$i] . ", ");
                    }
                    $msg .= ("and " . $department_names_array[$i] . " ");
                    $msg .= ("are divisions at " . $row["company_name"] . " already in the database ");
                    $msg .= ("with similar names to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to add this divisions to the database?<br /><br />");
                }

                promptAddDepartment($msg, $department_name, $department_website, $department_description, $job_id, $employer_id);
            }
        }

        if ($error == 0)
        {
            $sql = ("
                    INSERT INTO employer_department
                    (department_name, department_website, department_description,
                     entered_by, entered_on, employer_id)
                    VALUES
                    (
                     '" . addslashes(removeSlashes(trim($department_name))) . "',
                     '" . addslashes(removeSlashes(trim(normalizeWebsite($department_website)))) . "',
                     '" . addslashes(removeSlashes(trim($department_description))) . "',
                     '" . addslashes($auth->login) . "',
                     CURRENT_DATE,
                     '" . addslashes(removeSlashes($employer_id)) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS department_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $job->employer_id = $employer_id;
            $job->department_id = $row["department_id"];

            // Update the status for this company for each department at our organization.  
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $dept_result = $GLOBALS['dbh']->Execute($sql);
            while ($dept_row = $dept_result->FetchRow())
            {
                updateDivisionStatus($row['department_id'],$dept_row['department_id']);
                updateCompanyStatus($employer_id, $dept_row['department_id']);
            }

            /*
             Unset the website, and description for the job object, as well as any contact, and company 
             variables that have been set so that edit_contact_form pulls the values out of 
             the database for the newly created division.
             */

            unset($job->company_website);
            unset($job->company_description);
            unset($job->department_website);
            unset($job->department_description);
            unset($job->contact_id);
            unset($job->contact_phone);
            unset($job->contact_cellphone);
            unset($job->contact_fax);
            unset($job->contact_email);

            $order = $company_name[0];
            include('job_descriptions/edit_job/office/edit_contact_form.inc');
        }
    }
    if ($add_this_object == 'contact')
    {

        /*
         Validate the user inputs here.
         */
        if (trim($contact_first_name) == '')
        {
            $error = 1;
            error("You must supply a first name for the new contact.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif (trim($contact_last_name) == '')
        {
            $error = 1;
            error("You must supply a last name for the new contact.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif ((trim($contact_email) != '') && !(isValidEmail($contact_email)))
        {
            $error = 1;
            error("You must include a valid set of e-mail addresses, or none at all.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif ((trim($contact_phone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_phone)))))
        {
            $error = 1;
            error("You must include a valid phone number with an area code, or none at all.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif ((trim($contact_cellphone) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_cellphone)))))
        {
            $error = 1;
            error("You must include a valid cellphone number with an area code, or none at all.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif ((trim($contact_fax) != '') && !(ereg('(.*[0-9].*){8,}',(trim($contact_fax)))))
        {
            $error = 1;
            error("You must include a valid fax number with an area code, or none at all.");
            include('job_descriptions/edit_job/office/add_new_entity.inc');
        }
        elseif (!$add_confirmed)
        {

            /*
             Check in the database for any contacts at this company/division with similar names, and ask the
             user if they are sure they want to add THIS contact.  First check if we have a matching phone number.
             If we do, then display all contacts that share the last initial as this one, and check to make sure
             the user wishes to add this contact.  If no matching phone number, then show the user all contacts
             with similar last names to this contact and prompt for confirmation.
             */

            $contact_last_name = removeSlashes(trim($contact_last_name));

            $check_name = $contact_last_name[0] . $contact_last_name[1] . $contact_last_name[2];
            $check_initial = $contact_last_name[0];



            if (trim($contact_phone) != '')
            {
                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                        FROM contact AS a, employer_company AS b, contact_employer AS c
                        WHERE a.last_name LIKE '" . $check_initial . "%'
                        AND a.phone='" . addslashes(removeSlashes(trim($contact_phone))) . "'
                        AND c.employer_id='" . $employer_id . "'
                        AND a.contact_id=c.contact_id
                        AND b.employer_id=c.employer_id
                        ");
                $result  = $GLOBALS['dbh']->Execute($sql);
            }

            if (!($result->RecordCount()) || (trim($contact_phone) == ''))
            {
                $sql = ("
                        SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                        FROM contact AS a, employer_company AS b, contact_employer AS c
                        WHERE a.last_name LIKE '" . $check_name . "%'
                        AND c.employer_id='" . $employer_id . "'
                        AND a.contact_id=c.contact_id
                        AND b.employer_id=c.employer_id
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ($row["name"]);
                    $msg .= (" is an exisiting contact at " . $row["company_name"] . " already in the database ");
                    $msg .= (" with a name similar to the one you are entering.");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $contact_names_array[] = $row["name"];
                    }
                    for ($i = 0; $i < (sizeof($contact_names_array) - 1); $i++)
                    {
                        $msg .= ($contact_names_array[$i] . "<br />");
                    }
                    $msg .= ($contact_names_array[$i] . "<br /><br />");
                    $msg .= ("are contacts at " . $row["company_name"] . " already in the database ");
                    $msg .= ("with similar names to the one you are entering.");
                }
                $msg .= ("<br /><br />Are you sure you want to add this contact to the database?<br /><br />");
                promptAddContact($msg, $contact_name, $contact_phone, $contact_cellphone, $contact_fax, $contact_email, $job_id, $employer_id, $department_id);

            }

        }

        if ($error == 0)
        {
            /*
             If there is no division, then the department_id will == 'none'.  Set it to nothing.
             */

            if ($department_id == 'none')
            {
                $department_id = '';
            }
            $sql = ("
                    INSERT INTO contact
                    (type, first_name, last_name, email, phone, cellphone, fax)
                    VALUES
                    (
                     'employer',
                     '" . addSlashes(removeSlashes(trim($contact_first_name))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_last_name))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_email))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_phone))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_cellphone))) . "',
                     '" . addSlashes(removeSlashes(trim($contact_fax))) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            $sql = ("
                    SELECT LAST_INSERT_ID() AS contact_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $contact_id = $row["contact_id"];

            // Save pertinent information into contact_employer table.
            $sql = ("
                    INSERT INTO contact_employer
                    (contact_id, department_id, employer_id, entered_on, entered_by)
                    VALUES
                    (
                     '" . $contact_id . "',
                     '" . addSlashes(removeSlashes(trim($department_id))) . "',
                     '" . addSlashes(removeSlashes(trim($employer_id))) . "',
                     NOW(),
                     '" . addSlashes($auth->login) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            // Update the status for this contact for each department at our organization.  
            $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
            $dept_result = $GLOBALS['dbh']->Execute($sql);
            while ($dept_row = $dept_result->FetchRow())
            {
                // Give new contact potential lead
                updateContactStatus($row['contact_id'],$dept_row['department_id'],null,null);
            }


            $sql = ("
                    SELECT DISTINCT company_name
                    FROM employer_company
                    WHERE employer_id='" . $employer_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();
            $company_name = $row["company_name"];

            $job->employer_id = $employer_id;
            $job->department_id = $department_id;
            $job->contact_id = $contact_id;

            /*
             Unset the phone, fax, and e-mail for the job object, as well as any division
             and company variables that are set so that edit_contact_form pulls the values 
             out of the database for the newly created contact.
             */

            unset($job->company_website);
            unset($job->company_description);
            unset($job->department_website);
            unset($job->department_description);
            unset($job->contact_phone);
            unset($job->contact_cellphone);
            unset($job->contact_fax);
            unset($job->contact_email);

            $order = $company_name[0];
            $enable_save = 'true';
            include('job_descriptions/edit_job/office/edit_contact_form.inc');
        }
    }
}

elseif ($continue == 'Save Changes')
{
	/*
	 If we're returning from the promptContactChange function, we want to restore the job object.
	*/
	
	if ($check_contact)
	{
		/*
		 If $check_contact is true, then the job is in packed state.  We need to restore it.
		*/
		
        if (is_string($job)){
            $job = unpackObject($job);
        }

		$job->apply_method = $apply_method;

		if ($job->apply_method == JOB_APP_METHOD_ONLINE)
		{
			$job->receive_address = $receive_address;
		}
		else
		{
			$job->receive_address = '';
		}
	}
	else
	{
		$job = new EditJob('');
		$job->loadValues($job_id);
		$job->set_value_3($company_name, $company_description, $company_website, $employer_id, $department_name,
        	$department_description, $department_website, $department_id, $contact_name, $contact_phone, $contact_cellphone, $contact_email,
        	$contact_fax, $contact_id);
	}
	
	
	if (!$check_contact)
	{
        /*
         Now we need to check for a couple things.  If the contact_id for this job has changed, or the 
		 contact_id is the same, but the contact's e-mail has changed, we need to ask the user if they
		 would like to keep the same method of receiving applications, and if they would like to change
		 the address to receive these applications at.
		*/
	
		$sql = ("
			SELECT contact_id, receive_address, apply_method
			FROM job_info
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		
        if($job->department_id == 'none' || $job->department_id =='')
        {
            $error = 1;

            $msg .= ("You must select a department");
            $msg .= ("<br /><br />");

            promptDivisionchange($msg, $job_id);
        }
        elseif($job->contact_id != $row["contact_id"])
        {
            $error = 1;
	
			$msg .= ("You have changed the contact for this job.  Below are the current ");
			$msg .= ("method of application and e-mail address to receive online applications ");
			$msg .= ("at (if there is one).  Please make any desired changes and press continue.");
			$msg .= ("<br /><br />");

			promptContactChange($msg, $row["receive_address"], $row["apply_method"], $job, $job_id);
		}
		else
		{
			$sql = ("
				SELECT a.email, b.receive_address, b.apply_method
				FROM contact AS a, job_info AS b
				WHERE a.contact_id='" . $job->contact_id . "'
				AND b.job_id='" . $job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			
			if ($job->contact_email != $row["email"])
			{
				$error = 1;
	
				$msg .= ("You have changed the contact's e-mail for this job.  Below are the current ");
				$msg .= ("method of application and e-mail address to receive online applications at ");
				$msg .= ("(if there is one).  Please make any desired changes and press continue.");
				$msg .= ("<br /><br />");
			
				promptContactChange($msg, $row["receive_address"], $row["apply_method"], $job, $job_id);
			}
		}
	}

	if ($error == 0)
	{
		include('job_descriptions/edit_job/office/update_contact.inc');
		$job = new EditJob('');
        $job->loadValues($job_id);

        $no_buttons = true;
        include('job_descriptions/add_job/confirm_inputs.inc');
	}
}

/*
 End of edit_contact.inc code.
*/

/*
 Function below prompts user for confirmation to add a company that has a similar name in the database.
*/

function promptAddCompany($msg, $company_name, $company_website, $company_description, $job_id)
{
        global $PHP_SELF;
        error($msg .  "
                <table cellspacing=2 cellpadding=2>
                <tr>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='edit_job' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='level1' value='contact' />
                        <input type='hidden' name='add_this_object' value='employer' />
                        <input type='hidden' name='add_confirmed' value='true' />
                        <input type='hidden' name='continue' value='save_new_entity' />
                        <input type='hidden' name='company_name' value='" . $company_name . "' />
                        <input type='hidden' name='company_website' value='" . $company_website . "' />
                        <input type='hidden' name='company_description' value='" . $company_description . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='edit_job' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='level1' value='contact' />
                        <input type='hidden' name='add_this_object' value='employer' />
                        <input type='hidden' name='continue' value='add_new_entity' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form>
                </td>
                </tr>
                </table>");
}

function promptDivisionchange($msg, $job_id)
{
    $sql = ("
            SELECT DISTINCT department_name, department_id
            FROM employer_department
            WHERE employer_id='" . $job->employer_id . "'
            ORDER BY department_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    global $PHP_SELF;
    echo("
            <table cellspacing='2' cellpadding='2' class='row1'>
            <tr>
            <td align='center'>
            $msg
            <br />
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_job' />
            <input type='hidden' name='job_id' value='" . $job_id . "' />
            <input type='hidden' name='level1' value='contact' />
            <input style='width: 5em;' type='submit' value='&nbsp;BACK&nbsp;' />
            </form>
            </td>
            </tr>
            </table>");
}
function promptAddDepartment($msg, $department_name, $department_website, $department_description, $job_id, $employer_id)
{
    global $PHP_SELF;
    error($msg .  "
            <table cellspacing=2 cellpadding=2>
            <tr>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_job' />
            <input type='hidden' name='job_id' value='" . $job_id . "' />
            <input type='hidden' name='level1' value='contact' />
            <input type='hidden' name='add_this_object' value='department' />
            <input type='hidden' name='add_confirmed' value='true' />
            <input type='hidden' name='continue' value='save_new_entity' />
            <input type='hidden' name='department_name' value='" . $department_name . "' />
            <input type='hidden' name='department_website' value='" . $department_website . "' />
            <input type='hidden' name='department_description' value='" . $department_description . "' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
            </form>
            </td>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_job' />
            <input type='hidden' name='job_id' value='" . $job_id . "' />
        <input type='hidden' name='level1' value='contact' />
        <input type='hidden' name='add_this_object' value='employer' />
        <input type='hidden' name='continue' value='add_this_entity' />
        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
        </form>
        </td>
        </tr>
        </table>");
}

function promptAddContact($msg, $contact_name, $contact_phone, $contact_cellphone, $contact_fax, $contact_email, $job_id, $employer_id, $department_id)
{
    global $PHP_SELF;
    error($msg .  "
            <table cellspacing=2 cellpadding=2>
            <tr>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_job' />
            <input type='hidden' name='job_id' value='" . $job_id . "' />
            <input type='hidden' name='level1' value='contact' />
            <input type='hidden' name='add_this_object' value='contact' />
            <input type='hidden' name='add_confirmed' value='true' />
            <input type='hidden' name='continue' value='save_new_entity' />
            <input type='hidden' name='contact_name' value='" . $contact_name . "' />
            <input type='hidden' name='contact_phone' value='" . $contact_phone . "' />
            <input type='hidden' name='contact_cellphone' value='" . $contact_cellphone . "' /> 
            <input type='hidden' name='contact_fax' value='" . $contact_fax . "' />
            <input type='hidden' name='contact_email' value='" . $contact_email . "' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input type='hidden' name='department_id' value='" . $department_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
            </form>
            </td>
            <td>
            <form method='post' action='$PHP_SELF'>
        <input type='hidden' name='select' value='edit_job' />
        <input type='hidden' name='job_id' value='" . $job_id . "' />
        <input type='hidden' name='level1' value='contact' />
        <input type='hidden' name='add_this_object' value='contact' />
        <input type='hidden' name='continue' value='add_this_entity' />
        <input type='hidden' name='employer_id' value='" . $employer_id . "' />
        <input type='hidden' name='department_id' value='" . $department_id . "' />
        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
        </form>
        </td>
        </tr>
        </table>");
}

function promptContactChange($msg, $receive_address, $apply_method, $job, $job_id)
{
    global $PHP_SELF;

    $msg .= ("
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_job' />
            <input type='hidden' name='job_id' value='" . $job_id . "' />
            <input type='hidden' name='job' value='" . packObject($job) . "' />
            <input type='hidden' name='level1' value='contact' />
            <input type='hidden' name='continue' value='Save Changes' />
            <input type='hidden' name='check_contact' value='true' />
            <table cellspacing='2' cellpadding='2'>
           <tr>
            <td align='right'>
            Medium to receive applications:
            </td>
            <td align='left'>
            <input type='radio' class='row1' name='apply_method' value='".JOB_APP_METHOD_ONLINE."' ");
    if ($apply_method == JOB_APP_METHOD_ONLINE)
    {
        $msg .= ("checked='checked'");
    }
    $msg .= ("	
             />Online&nbsp;
            <input type='radio' class='row1' name='apply_method' value='".JOB_APP_METHOD_PAPER."' ");
            if ($apply_method == JOB_APP_METHOD_PAPER)
            {
                $msg .= ("checked='checked'");
            }
            $msg .= ("
                    />Paper&nbsp;
                    </td>
                    </tr>
                    <tr>
                    <td align='right'>
                    E-mail address for online applications:
                    </td>
                    <td align='left'>
                    <input type='text' name='receive_address' size='40' value='" . $receive_address . "' />
                    </td>
                    </tr>
                    <tr>
                    <td colspan='2' align='center'>
                    <input type='submit' value='Continue' />
                    </td>
                    </tr>
                    </table>
                    </form>");

            echo("
                    <table width='100%' class='row1' cellpadding='5' cellspacing='0' border='1'>
                    <tr>
                    <td>" . $msg . "</td>
                    </tr>
                    </table>
                    ");
}
?>
