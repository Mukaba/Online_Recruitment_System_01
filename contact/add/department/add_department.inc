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
 | Filename: add_department.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the file that does most of the decision making          |
 | involved with adding a division.  It decides whether or not the user has     |
 | filled out the division form, and if they haven't, it displays the form.     |
 | If they have filled out the form, then it checks all of the inputs to make   |
 | sure they are valid, checks and makes sure they aren't adding a division     |
 | for this company that isn't already in the database, and prompts them for    |
 | confirmation to add the division.                                            |
 +------------------------------------------------------------------------------+

*/


$error = 0;

if (!$employer_id)
{
	/*
	 The user has not chosen a company to add a division for yet, so show them the list
	 to choose from.
	*/
	
	include('contact/add/department/company_list.inc');
}

elseif ($employer_id && !$continue)
{
	/*
	 The user has chosen a company to add the division to, but hasn't filled out
	 the form, so show them the form.
	*/

	include('contact/add/department/add_department_form.inc');
}

elseif ($employer_id && $continue == 'Continue')
{
	/*
	 Fix the address to be on subsequent lines.
	*/

	if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
	{
		$department_street_address_2 = trim($department_street_address_3);
		$department_street_address_3 = '';
	}
	if (trim($department_street_address_1) == '' && trim($department_street_address_2) != '')
	{
		$department_street_address_1 = trim($department_street_address_2);
		$department_street_address_2 = '';
	}
    if (trim($department_street_address_2) == '' && trim($department_street_address_3) != '')
    {
            $department_street_address_2 = trim($department_street_address_3);
            $department_street_address_3 = '';
    }

    //Change "s to 's in the name
    $department_name = preg_replace("/\"/", "'", $department_name);

	/*
	 Trim all the empty elements out of the department_flags array.
	*/
	
	$department_flags = trim_array($department_flags);

	/*
	 Save the inputs into the contact object.
	*/

	$contact = new AddContact('');
	$contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

	/*
	 Begin validating inputs here.
	*/

	if (trim($contact->department_name) == '')
	{
		$error = 1;
		error("To add a new division, you must specify a division name.");
		include('contact/add/department/add_department_form.inc');
	}
	elseif ((trim($contact->department_email) != '') && !(isValidEmail($contact->department_email)))
	{
		$error = 1;
		error("You must include a valid set of e-mail addresses, or none at all.");
		include('contact/add/department/add_department_form.inc');
	}
	elseif ((trim($contact->department_phone) != '') && (trim($contact->department_phone) != USE_COMPANY) && !(ereg('(.*[0-9].*){8,}',(trim($contact->department_phone)))))
	{
		$error = 1;
		error("You must include a valid phone number with an area code, or none at all.");
		include('contact/add/department/add_department_form.inc');
	}
	elseif ((trim($contact->department_fax) != '') && (trim($contact->department_fax) != USE_COMPANY) && !(ereg('(.*[0-9].*){8,}',(trim($contact->department_fax)))))
	{
		$error = 1;
		error("You must include a valid fax number with an area code, or none at all.");
		include('contact/add/department/add_department_form.inc');
	}
	elseif (!trim($contact->department_industry_id))
	{
		$error = 1;
		error("You must choose an industry for the division you are adding.");
		include('contact/add/department/add_department_form.inc');
	}
    elseif (!trim($contact->department_size_id))
    {
        $error = 1;
        error("You must choose a size for the division you are adding.");
        include('contact/add/department/add_department_form.inc');
    }
    /*
     :TODO: Test that this code isn't needed, and then remove.
     - Division statuses are automatically set as CONTACT_POTENTIAL_LEAD upon entry now.
	elseif (!trim($contact->department_status))
    {
            $error = 1;
            error("You must choose a status for the division you are adding.");
            include('contact/add/department/add_department_form.inc');
    }
    */
	elseif (!$add_confirmed)
    {       
            
            /*
             Check in the database for any divisions at this company with similar names, and ask the user if they are
             sure they want to add THIS division.
            */

            $check_name = $contact->department_name[0] . $contact->department_name[1] . $contact->department_name[2];

            $sql = ("
                    SELECT DISTINCT a.department_name, b.company_name
                    FROM employer_department AS a, employer_company AS b
                    WHERE a.department_name LIKE '" . $check_name . "%'
        AND a.employer_id='" . $contact->department_company_id . "'
        AND b.employer_id='" . $contact->department_company_id . "'
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
            $msg .= ("<br /><br />Are you sure you want to add this division to the database?<br /><br />");
                    }

                    promptAddDepartment($msg, $contact);
            }
    }

    if ($error == 0)
    {
            include('contact/add/department/confirm_department_inputs.inc');
    }

}

elseif ($continue == 'add_confirmed')
{
        if ($add_confirmed && is_string($contact))
        {
                $contact = unpackObject($contact);
        }
        include('contact/add/department/confirm_department_inputs.inc');
}

elseif ($continue == 'Add Division Now')
{
	$contact = unpackObject($contact);
	include('contact/add/department/save_department.inc');
}

elseif ($continue == 'Use Company Fax')
{       
        /*
         They want to use the company's fax number for this division.
         So simply reload the page.
        */      
        
        $department_fax = USE_COMPANY;
	
	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3, 
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

        include('contact/add/department/add_department_form.inc');
}               
                
elseif ($continue == 'Specify division fax')
{       
        /*
         They want to stop using the company fax number and specify
         one for the division.
        */      
                 
        $department_fax = ''; 

	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

        include('contact/add/department/add_department_form.inc');
}

elseif ($continue == 'Use Company Phone')
{
	/*
	 They want to use the company's phone number for this division.
	 So simply reload the page.
	*/
	
	$department_phone = USE_COMPANY;

	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3, 
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

	include('contact/add/department/add_department_form.inc');
}

elseif ($continue == 'Specify division phone')
{
	/*
	 They want to stop using the company phone number and specify
	 one for the division.
	*/

	$department_phone = '';

	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3, 
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

	include('contact/add/department/add_department_form.inc');
}

elseif ($continue == 'Use Company Location')
{
        /*
         They want to use the company's location for this division, so simply reload the page.
        */

        $department_location_info = USE_COMPANY;

	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3, 
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);
	
        include('contact/add/department/add_department_form.inc');
}

elseif ($continue == 'Specify division location')
{
        /*
         They want to stop using the company information and specify the division's own.
        */

        $department_location_info = '';
        $department_street_address_1 = '';
        $department_street_address_2 = '';
	$department_street_address_3 = '';
        $department_country_id = '';
        $department_provstate_id = '';
        $department_region_id = '';
        $department_city = '';
        $department_postal_code = '';

	$contact = new AddContact('');
        $contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
        $department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
	$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code, 
	$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id, 
	$department_new_note, $department_department_comments, $department_status);

        include('contact/add/department/add_department_form.inc');
}

elseif ($continue == 'Add Division Now')
{
        $contact = unpackObject($contact);
        include('contact/add/department/save_department.inc');
}

elseif ($continue == 'Edit Division')
{
        $contact = unpackObject($contact);
        include('contact/add/department/add_department_form.inc');
}

?>
