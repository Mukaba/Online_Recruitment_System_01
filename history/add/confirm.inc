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
 | Filename: confirm.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: A final confirmation before you add a history record into the   |
 | database.                                                                    |
 +------------------------------------------------------------------------------+

*/

// Display appropriate header and form action: 'own job placement' and 'returning placement' function synonymously
// only diff between them is header and page re-routes
if($select == "add_history")
{
    echo("<h3 align='left'>Job Offers - Own Job Placement</h3>");
    echo("<form action='" .$PHP_SELF. "&amp;select=add_history' method='post'>");
}
if($select == "returning_placement")
{
    echo("<h3 align='left'>Job Offers - Returning Job Placement</h3>");
    echo("<form action='" .$PHP_SELF. "&amp;select=returning_placement' method='post'>");
}
?>
<table cellspacing='0' cellpadding='5' border='0'>
	<tr>
		<td><b>Company Name:</b></td>
		<td>&nbsp;<?php echo($addHistory->company_name); ?></td>
	</tr>
	<tr>
		<td><b>Division Name:</b></td>
		<td>&nbsp;<?php echo($addHistory->division_name); ?><?php if ($addHistory->city) { echo(" (".$addHistory->city.")"); } ?></td>
	</tr>
	<tr>
		<td><b>Student:</b></td>
		<td>&nbsp;<?php echo($addHistory->first_name." ".$addHistory->last_name); ?></td>
	</tr>
	<tr>
		<td><b>Term:</b></td>
		<td>&nbsp;<?php echo(getTermName($addHistory->term_id)." ".$addHistory->year); ?></td>
	</tr>
    <?php 

    if ($addHistory->container_mode)
    {
    ?>
        <tr>
            <td><b>Start Date:</b></td>
            <td>&nbsp;<?php echo($addHistory->start_date." (YYYY-MM-DD)"); ?></td>
        </tr>

    <?php
    }

    if ($addHistory->container_mode == CONTAINER_TERM)
    {
    ?>
        <tr>
            <td><b>Work Term Length:</b></td>
            <td>&nbsp;<?php echo($addHistory->work_term_length." month"); if ($addHistory->work_term_length != 1) { echo("s"); } ?></td>
        </tr>

    <?php
    }

    elseif ($addHistory->container_mode == CONTAINER_HOURS)
    {
    ?>
        <tr>
            <td><b>End Date:</b></td>
            <td>&nbsp;<?php echo($addHistory->end_date. " (YYYY-MM-DD)"); ?></td>
        </tr>

    <?php
    }

    if ($addHistory->container_mode)
    {
    ?>
        <tr>
            <td><b>Hours Per Week:</b></td>
            <td>&nbsp;<?php echo($addHistory->work_term_hours_per_week); ?></td>
        </tr>

    <?php
    }

    ?>
</table>
<p>
<table class='row0' cellspacing='0' border='0' cellpadding='5'>
    <tr>
        <td><input checked type='radio' value='save_and_edit' name='level' /> Yes, this looks fine. I'd like to edit this record after saving this.</td>
    </tr>
    <tr>
        <td><input type='radio' value='save_and_new' name='level' /> Yes, this looks fine.  I'd like to add another record after after saving this.</td>
    </tr>
    <tr>
        <td><input type='radio' value='dont_save_and_new' name='level' /> No, don't save this, take me back to the original add screen.</td>
    </tr>
    <input type='hidden' name='addHistory' value='<?php echo packObject(($addHistory)) ?>' />
</table>
</p>
<p>
<input type='submit' value='Submit' name='submit' />
</p>
</form>
