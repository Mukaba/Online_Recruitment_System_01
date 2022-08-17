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
 | Filename: student_chooser.php                                                |
 +------------------------------------------------------------------------------+
 | Description: This file allows the user to choose a student from a list       |
 | shown in a seperate pop-up window.  Once selected, the student name is sent  |
 | back to the window that opened this file, and this files window is closed.   |
 +------------------------------------------------------------------------------+

*/
?>

<script type='text/javascript' language='javascript'>

<!--javascript

function unfocus()
{
        self.close();
}

function addStudent(StudentName)
{
        var x = document.form1.student_list_slot.value;
	var formName = document.form1.parentFormName.value;

	var has_names = eval("window.opener.document." + formName + ".elements[" + x + "].value");

	// if the length is greater than 0, and if not on a newline, we need one
	// for some reason Windows uses '10' whereas Macs use '13' for the character code
	var last_char = has_names.charCodeAt(has_names.length-1);
	
	if ((has_names.length == 0) || ((has_names.length > 0) && ((last_char == 13) || (last_char == 10))))
		var new_name = StudentName;
	else
		var new_name = String.fromCharCode(13) + StudentName;

        str = "window.opener.document." + formName + ".elements[" + x + "].value += new_name";
	eval(str);
        self.close();
}

function resizeOuterTo(w,h) 
{
    <?php 
        $w3client = new wwwclient;
        $w3client->wwwclient();
        $browser_id = $w3client->browser;
    ?>
    var pattern = /X11/;
    var result;
    var browser = <?php echo $browser_id;?>;
    
    result=pattern.exec(navigator.appVersion); 
    if (result != null) 
    {
        window.resizeTo((w + 280),(h + 120));
    }  

    w = document.getElementById('SC').offsetWidth;
    h = document.getElementById('SC').offsetHeight;

    if(browser ==<?php echo NETSCAPE; ?>) 
    {
        // If browser is Netscape...
        // w = w + 150;
        // h = h + 50;
        w = w + 20;
        h = h + 30;
    }
    // If browser is Internet Explorer...
    if(browser == <?php echo IE; ?>)                    
    {   
        // w = w + 180;
        // h = h + 10;
        w = w + 40;
        h = h + 25;       
    }
    // If browser is Gecko derived...
    if(browser == <?php echo GECKO; ?>)                    
    {   
        // w = w + 150;
        // h = h + 70;
    }
    // If browser is Lynx...
    if(browser == <?php echo LYNX; ?>)                    
    {   
        // w = w + 150;
        // h = h + 50;
    }
    // If browser is Safari...
    if(browser == <?php echo SAFARI; ?>)                    
    {   
        // w = w + 160;
        // h = h + 45;
        w = w + 30;
        h = h + 50;
    }
    // If browser is Firefox...
    if(browser == <?php echo FIREFOX; ?>)
    {   
        // w = w + 260;
        // h = h + 50;
        w = w + 30;
        h = h + 30;
    }
    else
    {    
        //w = w + 100;
        //h = h + 180;
    }
    if(navigator.platform == "Win32")
    {
       w = w + 20;
       h = h + 30;   
    }
    //main resizing
    window.resizeTo(w,h);
}

//-->


</script></head><body bgcolor='#ffffff'>
<?php print "<form name='form1' method='post' action='mamook.php?select=student_chooser&amp;no_headers=1&amp;department_selected=$department_selected&amp;student_list_slot=$student_list_slot&amp;parentFormName=$parentFormName>"; ?>

<!-- 'page table' -->
<table border='0' width='100%' height='100%' align='left' valign='top'>
<tr align='center'>
<td align='center'>
		
	<table id='SC' name='mainTable' border='0' cellpadding='5' cellspacing='0' class='row1'>
		
		<tr>
			<td align='center' colspan='2' nowrap='nowrap'>
			<b class='black'>Choose a student:</b>
			</td>
		</tr>
		
		<tr>
			<td colspan='2'>
			

<?php

// for the department drop down

if ($department_selected == "") {
	
	$sql = "Select pulldown_menu_group from contact_internal where department_id='". $department_id ."' and (login_id='". $login_id. "' or netlink_id='". $login_id ."')";
	$results = $GLOBALS['dbh']->Execute($sql);
	$row = $results->FetchRow();

	if ($row['pulldown_menu_group'] == 1) {
		$department_selected = 0;
	}

	else {
		$department_selected = $department_id;
	}
}

if ($department_selected == 0) {
	$department_in_str = "'" . implode($departments_in_group, "', '") . "'";
}
else {
	$department_in_str = "'" . $department_selected . "'";
}


if (sizeof($departments_in_group) > 1)	{
?>
	<table border="0" width="100%" class="row0" cellspacing="0" cellpadding="5">
		<tr>
			<td align="right">Department:</td>
			<td><select name="department_selected">
			<option value="0" <?php echo ($department_selected == 0) ? "selected='selected'" : ""; ?>>All in Group</option>
<?php

			foreach($departments_in_group as $dep) {
				echo "\t<option value='$dep'" . (($dep == $department_selected) ? "selected='selected'" : "" ) . ">" . getDepartmentName($dep) . "</option>\n";
			}
?>
			</select>
			&nbsp;&nbsp;<input type="submit" Name="changetodepartment" value="Change" /></td>
		</tr>
	</table>

	<br />
<?php
}

$letter_href="mamook.php?select=student_chooser&no_headers=1&department_selected=$department_selected&student_list_slot=$student_list_slot&parentFormName=$parentFormName";

echo ("<input type='hidden' name='student_list_slot' value='$student_list_slot'>");
echo ("<input type='hidden' name='parentFormName' value='$parentFormName'>");
?>
<table width="100%" class="row0" cellspacing="0" cellpadding="5" border="0">
<!--	<tr>
		<td colspan="2" align="LEFT" class="tableheaders">Enter a Student Number</td>
	</tr>
	
	<tr>
		<td width="30%" align="RIGHT">Student Number:</td>
		<td align="LEFT"><input type="TEXT" size="14" name="student_number_select"></td>
	</tr>

	<tr>
		<td colspan="2" align="LEFT" class="tableheaders">Or Select a Student</td>
	</tr> -->

	<tr>
		<td colspan="2" align="center">
		&nbsp;<br />
		<?php
            
		$letter_sql="SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS letter
			FROM student AS s
            LEFT JOIN student_department AS sd ON s.student_number = sd.student_number
            LEFT JOIN student_flags_join AS sfj ON sfj.record_id=sd.record_id
			WHERE sd.department_id IN (".$department_in_str.")
            AND sfj.student_flags_id = ".CURRENT_FLAG."
			ORDER BY s.last_name";

		$letter_results = $GLOBALS['dbh']->Execute($letter_sql);

		$letters_total = 0;
        
        $letter_row = $letter_results->FetchRow();
        if ($letter_order == "")
            $letter_order = $letter_row[0];
        
		while ($letter_row = $letter_results->FetchRow()) { 
			
            $pass_letter = strtoupper($letter_row["letter"]);
		   
            // the letter selected when popup is first opened
			// for some reason the above query brings back everybody (or so it seems
			// not just the distinct rows of the letters, so check to see if it's already
			// in there to display or not
			
			
            if ($pass_letter != $prev_letter) {
				$letters_total ++;
				
				$prev_letter = $pass_letter;
		
                if ($pass_letter != $letter_order)
					echo ("<a href='$letter_href&letter_order=$pass_letter'>");
			    
				echo ($pass_letter);
			
				if ($pass_letter != $letter_order)
					echo ("</a>");

				echo ("&nbsp;|&nbsp;");

			}
            
		}
        
        echo ("<input type='hidden' name='letter_order' value='$letter_order'>");
		// needed to set the width, javascript will only understand if it's done through a form
		echo ("<input type='hidden' name='letter_width' value='$letters_total'>");

		$sql = "SELECT s.student_number, CONCAT(s.first_name,' ',s.last_name) as name, CONCAT(s.last_name, ', ', s.first_name) as pass_name
			FROM student AS s 
			LEFT JOIN student_department AS sd ON s.student_number = sd.student_number
            LEFT JOIN student_flags_join AS sfj ON sfj.record_id = sd.record_id
			WHERE (sd.department_id IN (".$department_in_str."))
			AND s.last_name LIKE '" . $letter_order . "%' 
			AND sfj.student_flags_id = ".CURRENT_FLAG." 
			ORDER BY s.last_name";
		$result = $GLOBALS['dbh']->Execute($sql);

		?>

		</td>
	</tr>

	<tr>
		<td colspan="2"  align="center">
		<?php
		$size=10;
		
		if ($result->RecordCount() < 10)
			$size = (($result->RecordCount() > 1) ? $result->RecordCount() : 2);
			
		?>
			<select name="students" onChange="addStudent(document.form1.students[document.form1.students.selectedIndex].value)" size="<?php echo $size?>">
		<?php
			while ($row = $result->FetchRow()) {
				echo ("<option value=\"". $row["pass_name"] ."\">[". $row["student_number"]. "] ". $row["name"] ."</option>");
			}
		?>
			</select>
		<?php
		?>
		</td>
	</tr>
</table>



<!-- 'page table' -->
		</td>
		</tr>
	</table>

</td>
</tr>
</table>


</form>

<script type='text/javascript' language='javascript'>

var heightOffSet = 0;
var widthOffSet = 0;

var widthMultiplier = 7;
var heightMultiplier = 12;

var selectboxLength = 0;

selectboxLength = (document.form1.students.length);

if (selectboxLength > 10)
{
	selectboxLength = 10;
}

var selectboxWidth = 0;

var i;

for (i = 0; i < document.form1.students.length; i++)
{
	curLength = document.form1.students[i].text.length;
	if (curLength > selectboxWidth)
	{
		selectboxWidth = curLength;
	}
}

selectboxWidth = selectboxWidth - 30;

if (selectboxWidth < 0)
{
	selectboxWidth = 0;
}

if (document.form1.letter_width.value > selectboxWidth)
{
	selectboxWidth = document.form1.letter_width.value;
	widthMultiplier = 9.0;
}

//widthOffSet = selectboxWidth * widthMultiplier;
//heightOffSet = selectboxLength * heightMultiplier;

widthOffSet = document.getElementById('SC').offsetWidth;
heightOffSet = document.getElementById('SC').offsetHeight;
    
resizeOuterTo((widthOffSet),(heightOffSet));

</script>
