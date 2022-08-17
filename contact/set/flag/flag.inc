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
 | Filename: flag.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This is the main flag file.  It determines which page it        |
 | should be loading, or whether or not it needs to save the flags for this     |
 | contact.                                                                     |
 +------------------------------------------------------------------------------+

*/


if (!$continue)
{
	// They haven't done anything yet, so display the general form for them.
	
	include('contact/set/flag/general_display.inc');
}

elseif ($continue == 'Save Changes')
{
	/*
	 They want to save some flags that they have set.  We have been passed back three variables from
	 the form: flag_id, which is the flag they were setting or unsetting, contact_ids, which is an
	 array containing all of the contact id's that they were setting and unsetting for, and
	 flag_on, which is an array containing the contact id's that have the flag set to on.
	*/

	if ($contact_ids && is_string($contact_ids))
	{
		$contact_ids = unpackObject(($contact_ids));
        $contact_ids_string = implode(",",$contact_ids);
	}

	// Trim out the blank elements of flag_on.

	$flag_on = trim_array($flag_on);

	// Delete all the instances of this flag and contact id from the database.

    $sql = ("
        DELETE FROM contact_flags_set
        WHERE contact_id IN (" . $contact_ids_string . ")
        AND flag_id='" . $contact_flag . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

	// Insert back in all of the new instances of this flag and contact id.

	if (trim($flag_on))
	{

		for ($i = 0; $i < sizeof($flag_on); $i++)
		{
			$sql = ("
				INSERT INTO contact_flags_set
				(contact_id, flag_id)
				VALUES 
				(
				'" . $flag_on[$i] . "',
				'" . $contact_flag . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
		
	// Reload the previous page.

	notify("The <b>" . getFlagName($contact_flag, "contact") . "</b> flag has been successfully saved for the specified contact(s).");
	include('contact/set/flag/general_display.inc');
}

elseif ($continue == 'Set Flag and Save for All Contacts on All Pages')
{
    // They would like to set the specified flag for ALL of the contacts that they have available.

    // The next page expects contact_ids to be unserialized and urldecoded, so do that here.
	if ($contact_ids && is_string($contact_ids))
	{
		$contact_ids = unpackObject(($contact_ids));
	}

    // First clear the flag for all of the contacts (so that we don't have it set more than once).
    $sql = ("
        DELETE FROM contact_flags_set
        WHERE contact_id IN (" . $contact_id_list . ")
        AND flag_id='" . $contact_flag . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    // Now save the flag for all of the contacts.
    $contact_id_array = explode(",", $contact_id_list);

    foreach ($contact_id_array as $cia)
    {
        $sql = ("
            INSERT INTO contact_flags_set
            (contact_id, flag_id)
            VALUES
            (
            '" . $cia . "',
            '" . $contact_flag . "'
            )
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }

    notify("The <b>" . getFlagName($contact_flag, "contact") . "</b> flag has been successfully set and saved for all of the available contacts.");
    include('contact/set/flag/general_display.inc');
}

elseif ($continue == 'Unset Flag and Save for All Contacts on All Pages')
{
    // They would like to unset the specified flag for ALL of the contacts that they have available.

    // The next page expects contact_ids to be unserialized and urldecoded, so do that here.
	if ($contact_ids && is_string($contact_ids))
	{
		$contact_ids = unpackObject(($contact_ids));
	}

    // Clear the specified flag for all of the available contacts.
    $sql = ("
        DELETE FROM contact_flags_set
        WHERE contact_id IN (" . $contact_id_list . ")
        AND flag_id='" . $contact_flag . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);

    notify("The <b>" . getFlagName($contact_flag, "contact") . "</b> flag has been successfully unset and saved for all of the available contacts.");
    include('contact/set/flag/general_display.inc');
}

elseif ($continue == 'show_specific')
{
	// The user would like to edit this contact's flags specifically.

	// Pull out all of the current flags that are set, so we can check for concurrent editing problems.
	$sql = ("
		SELECT DISTINCT a.flag_id
		FROM contact_flags_set AS a, department_contact_flags AS b
		WHERE b.department_id='" . $auth->department . "'
		AND a.contact_id='" . $contact_id . "'
		AND a.flag_id=b.flag_id
		ORDER BY a.flag_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	while ($row = $result->FetchRow())
	{
		$initial_flags_set[] = $row["flag_id"];
	}

	include('contact/set/flag/specific_display.inc');
}

elseif ($continue == 'save_specific')
{
	// The user would like to save the changes they have made to this contact's flags.

	$contact_flags = trim_array($contact_flags);
	$department_contact_flags = unpackObject(($department_contact_flags));

	// Do we have concurrent editing problems?

	if (isset($initial_flags_set) && is_string($initial_flags_set))
	{
		$initial_flags_set = unpackObject(($initial_flags_set));
	}

	$sql = ("
		SELECT DISTINCT a.flag_id
		FROM contact_flags_set AS a, department_contact_flags AS b
		WHERE b.department_id='" . $auth->department . "'
		AND a.contact_id='" . $contact_id . "'
		AND a.flag_id=b.flag_id
		ORDER BY a.flag_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	while ($row = $result->FetchRow())
	{
		$current_flags_set[] = $row["flag_id"];
	}

	if (is_array($current_flags_set) XOR is_array($initial_flags_set))
	{
		$concurrent_edit = TRUE;
	}
	elseif (is_array($current_flags_set))
	{
		// They must both be an array

		foreach($current_flags_set AS $cfs)
		{
			if (!in_array($cfs, $initial_flags_set))
			{
				$concurrent_edit = TRUE;
				break;
			}
		}
		foreach($initial_flags_set AS $ifs)
		{
			if (!is_array($current_flags_set))
			{
				$concurrent_edit = TRUE;
				break;
			}
		}
	}

	if ($concurrent_edit)
	{
		// Someone else has edited this contact's flags while the other person has been in.

		$flags_changed["flags_changed"] = TRUE;
		foreach ($contact_flags AS $cf)
		{
			$flags_changed["flags_set"][] = $cf;
		}

		$msg = ("Someone has changed the flags for this contact since you last loaded the page.");
		$msg .= ("<br /><br />The changes you have made have NOT been saved, but are shown in <b class='black'>bold</b> below the data that can be modified.");
                $msg .= ("  Once you have modified the contact as desired, press the 'Save Changes' button below to update the data.");

		notify($msg);
		// Pull out all of the current flags that are set, so we can check for concurrent editing problems.
		$sql = ("
			SELECT DISTINCT a.flag_id
			FROM contact_flags_set AS a, department_contact_flags AS b
			WHERE b.department_id='" . $auth->department . "'
			AND a.contact_id='" . $contact_id . "'
			AND a.flag_id=b.flag_id
			ORDER BY a.flag_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		while ($row = $result->FetchRow())
		{
			$initial_flags_set[] = $row["flag_id"];
		}

		include('contact/set/flag/specific_display.inc');
	}
	else
	{
		// Delete all of the old flags (that this department can set) for this contact from the database.

		for ($i = 0; $i < sizeof($department_contact_flags); $i++)
		{
			$sql = ("
				DELETE FROM contact_flags_set
				WHERE contact_id='" . $contact_id . "'
				AND flag_id='" . $department_contact_flags[$i] . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}

		// Insert all of the new flags that have been set for this contact into the database.

		for ($i = 0; $i < sizeof($contact_flags); $i++)
		{
			$sql = ("
				INSERT INTO contact_flags_set
				(contact_id, flag_id)
				VALUES
				(
				'" . $contact_id . "',
				'" . $contact_flags[$i] . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}

		unset($department_contact_flags);
		unset($contact_flags);

		// Notify that the changes have been successfully made, and reload the page.
		
		notify("Flags have been saved successfully.");

		// Pull out all of the current flags that are set, so we can check for concurrent editing problems.
		$sql = ("
			SELECT DISTINCT a.flag_id
			FROM contact_flags_set AS a, department_contact_flags AS b
			WHERE b.department_id='" . $auth->department . "'
			AND a.contact_id='" . $contact_id . "'
			AND a.flag_id=b.flag_id
			ORDER BY a.flag_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		while ($row = $result->FetchRow())
		{
			$initial_flags_set[] = $row["flag_id"];
		}

		include('contact/set/flag/specific_display.inc');
	}
}
	

?>
