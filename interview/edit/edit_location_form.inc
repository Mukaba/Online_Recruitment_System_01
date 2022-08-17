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
 | Filename: edit_location_form.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: this form comes up with all data previously entered showing,    |
 | the user has the opportunity to change the data.                             |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/
$sql = "SELECT building_code, building_id
        FROM building
	ORDER BY building_code";

$result = $GLOBALS['dbh']->Execute($sql);

$js_building_array = "Array(";
$js_buildingid_array = "Array(";
$js_room_id_array = "Array(";
$js_room_number_array = "Array(";

while ($row = $result->FetchRow()){
	$sql = "SELECT room_id, room_number
		FROM room
		WHERE building_id = '" . addslashes($row["building_id"]) . "'
		ORDER BY room_number";

	$result2 = $GLOBALS['dbh']->Execute($sql);
	if ($result2->RecordCount() > 0){
		$room_sub_array = "Array(";
		$room_sub_array2 = "Array(";

		while($row2 =  $result2->FetchRow()){
			$room_sub_array .= "'" . $row2["room_number"] . "',";
			$room_sub_array2 .= "'" . $row2["room_id"] . "',";
		}

		$room_sub_array = substr($room_sub_array, 0, strlen($room_sub_array) - 1) . ")";
		$room_sub_array2 = substr($room_sub_array2, 0, strlen($room_sub_array2) - 1) . ")";
		
		$js_room_id_array .= $room_sub_array2 . ",";
		$js_room_number_array .= $room_sub_array . ",";
		$js_building_array .= "'" . $row["building_code"] . "',";
		$js_buildingid_array .= "'" . $row["building_id"] . "',";
	}
}

$js_room_number_array = substr($js_room_number_array, 0, strlen($js_room_number_array) - 1) . ")";
$js_room_id_array = substr($js_room_id_array, 0, strlen($js_room_id_array) - 1) . ")";
$js_building_array = substr($js_building_array, 0, strlen($js_building_array) - 1);
$js_building_array .= ")";
$js_buildingid_array = substr($js_buildingid_array, 0, strlen($js_buildingid_array) - 1);
$js_buildingid_array .= ")";


?>

<script type="text/javascript" language="javascript">
<!--javascript
var room_ids = new <?php echo $js_room_id_array?>;
var rooms = new <?php echo $js_room_number_array?>;
var building_codes = new <?php echo $js_building_array?>;
var building_ids = new <?php echo $js_buildingid_array?>;

function changeRoom(loc_number){
	str = "document.form1.locations" + loc_number + ".selectedIndex == document.form1.wait_building" + loc_number + ".selectedIndex";
	var change1 = eval(str);

	if (change1){
		str = "document.form1.wait_room" + loc_number + ".selectedIndex = document.form1.room" + loc_number + ".selectedIndex";
		eval(str);
	}
}

function setbuilding(loc_number, room_index){
	if (room_index < 0 || room_ids[room_index].length <= 0){
		str = "document.form1.wait_room" + loc_number + ".options.length=1";
		eval(str);
		str = "document.form1.wait_room" + loc_number + ".options[0] = new Option(\"T . B . A\", \"\")";
		eval(str);
	}else{
		str = "document.form1.wait_room" + loc_number + ".length = " + room_ids[room_index].length;
		eval(str);
		str = "document.form1.wait_room" + loc_number + ".options.length = " + room_ids[room_index].length;
		eval(str);
		for(i=0;i<room_ids[room_index].length;i++){
			str = "document.form1.wait_room" + loc_number + ".options[" + i +"] = new Option(\"" + rooms[room_index][i] + "\", \"" + room_ids[room_index][i] + "\")";
			eval(str);
		}
	}
	str = "document.form1.wait_building" + loc_number + ".selectedIndex = document.form1.locations" + loc_number + ".selectedIndex";
	eval(str);
	str = "document.form1.wait_room" + loc_number + ".selectedIndex = document.form1.room" + loc_number + ".selectedIndex";
	eval(str);
}

function change_rooms(loc_number, select_box_name, change_name){
	var str = "document.form1." + select_box_name + loc_number + "[document.form1." + select_box_name + loc_number + ".selectedIndex].value";
	var num = eval(str);
	var i = 0;
	var room_index = -1;
	for(i=0;i<building_ids.length;i++){
		if (building_ids[i] == num){
			room_index = i;
			break;
		}
	}
	if (room_index < 0 || room_ids[room_index].length <= 0){
		str = "document.form1." + change_name + loc_number + ".options.length = 1";
		eval (str);
		str = "document.form1." + change_name + loc_number + ".options[0] = new Option(\"T . B . A\", \"\")";
		eval (str);
	}else{
		str = "document.form1." + change_name + loc_number + ".length = " + room_ids[room_index].length;
		eval (str);
		str = "document.form1." + change_name + loc_number + ".options.length = " + room_ids[room_index].length;
		eval (str);
		for(i=0;i<room_ids[room_index].length;i++){
			str = "document.form1." + change_name + loc_number + ".options[" + i + "] = new Option(\"" + rooms[room_index][i] + "\", \"" + room_ids[room_index][i] + "\")";
			eval(str);
		}
	}
	str = "document.form1." + change_name + loc_number + ".selectedIndex=0";
	eval(str);
	
	if (select_box_name == "locations"){
		setbuilding(loc_number, room_index);
	}
}
//-->
</script>


<form method="post" name="form1" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="edit" />
<input type="hidden" name="level1" value="location" />
<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
<input type='hidden' name='interview' value='<?php echo ( packObject(($interview)) ); ?>' />

<table border="0" cellpadding="4" cellspacing="0" class="row1" width="100%">
  <tr>
    <td align="right">Employer:</td>
    <td colspan="3"><?php echo($interview->company_name);?></td>
  </tr>

<?php
$sql = "SELECT *
        FROM interview_place";
$result = $GLOBALS['dbh']->Execute($sql);

?>
	        
 <tr>
    <td align="right">Interview Place:</td>
    <td colspan="3"><select name="interview_place">
	<?php
	while($row = $result->FetchRow()){
		echo "<option value='" . $row["interview_place_id"] . "'" . (($row["interview_place_id"] == $interview->interview_place) ? " selected='selected' " : "") . ">" . $row["description"] . "</option>\n";
		if ($edit_conflict && $row["interview_place_id"] == $interview2->interview_place){
			$place = $row["description"];
		}
	}
	?>
	</select>
	<?php
	if ($edit_conflict){
		echo "<br /><b class='black'>($place)</b>\n";
	}
	?>
     </td>
  </tr>
  <?php
    if (isPhone ($interview->interview_medium))
    {
      ?>
        <tr>
          <td colspan="4" align="center">
            <?php
              $is_checked = "";

              $sql = "select use_room_phone_number from interview where job_id=\"$job_id\"";
	      $response = $GLOBALS['dbh']->Execute ($sql);
	      $eh = $response->FetchRow();

	      if ($eh["use_room_phone_number"] == 1)
	        $is_checked = " checked='checked'";
            ?> 
              <input type="checkbox" name="use_room_number" <?php echo $is_checked?> />If available, give the employer 
                 the phone number for the room
                 that the telephone interview is in, rather than the default contact number.
          </td>
        </tr>
      <?php
    }
  ?>
  <tr>
    <td colspan="4"><hr width="100%" /></td>
  </tr>
<?php

{
	$multiple = 0;
      for ($i = 0; $i < count($interview->interviewers); $i++)
      {
         $interview_multiple = 0;
	 $sql = ("SELECT DISTINCT int_room_id, int_wait_room_id
	 		     FROM interview_time
			     WHERE interviewer_id = '" . $interview->interviewer_ids[$i] . "' AND 
			     	int_type = '" . INTERVIEW . "'");
	 $result = $GLOBALS['dbh']->Execute($sql);
	 if ($result->RecordCount() > 1){
         	$interview_multiple = 1;
		 $multiple = 1;
	 }
	 $row = $result->FetchRow();
	 
	 $default_room_id = $row["int_room_id"];
	 $default_wait_room_id = $row["int_wait_room_id"];

	 $result = $GLOBALS['dbh']->Execute("SELECT building_id
	 		 	FROM room
				WHERE room_id = '" . $default_room_id . "'");
	 $row = $result->FetchRow();
	 $default_building_id = $row["building_id"];

	 $result = $GLOBALS['dbh']->Execute("SELECT building_id
	 		 	FROM room
				WHERE room_id = '" . $default_wait_room_id . "'");
	 $row = $result->FetchRow();
	 $default_wait_building_id = $row["building_id"];
	


	 $j = $i + 1;
         
	 echo("<tr>\n");
         echo("<td align='right'>Interviewer $j:</td>\n");
         echo("<td colspan='3'>" . $interview->interviewers[$i] . "\n");
	     echo("</td>");   
     echo("</tr>\n");
         if ($interview_multiple){
		 echo "<tr><td colspan='4' align='center'>Multiple rooms have been specified for this interviewer.</td></tr>";
	 }
	 if ($edit_conflict && $interview2->location_names[$i] == "Many" && (isOnCampus($interview->interview_place) || isPhone($interview->interview_medium))){
		 echo "<tr><td colspan='4' align='center'><b class='black'>(You wished to specify multiple interviews for this interviewer.)</b></td></tr>";
	 }
	 if( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) )
         {

		echo("<tr><td align='right'>Interview Building $j:");
		if ($edit_conflict){
			echo "<br /><b class='black'>(" . $interview2->location_names[$i] . ")</b>";
		}
		echo ("</td>\n");
	        echo("<td>
			<select name='locations$i' onchange='change_rooms(\"$i\", \"locations\", \"room\")'>\n");
		echo( "<option value=''" . (($default_building_id == "") ? "selected='selected'" : "") . " >T . B . A</option>\n" );
		
		$result = $GLOBALS['dbh']->Execute( "SELECT DISTINCT b.building_id as building_id, b.building_code as building_code
					FROM room as a, building as b
					WHERE a.building_id=b.building_id" );
		while( ($row = $result->FetchRow()) )	
		{
			echo( "<option value='" . $row['building_id'] . ( ($row["building_id"] == $default_building_id) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>\n" );
		}
		echo "</select>
			</td>\n";
		echo "<td align='right'>Room $j:</td>\n";
		echo ("<td>
			<select name='room$i' onchange='changeRoom(\"$i\")'>\n");
        	
		$result = $GLOBALS['dbh']->Execute("SELECT room_id, room_number
					FROM room
					WHERE building_id = '" . $default_building_id . "'");
		if ($result->RecordCount() <= 0){
			echo "<option value='' selected='selected'>T . B . A</option>\n";
		}
		while ($row = $result->FetchRow())
		{
			echo( "<option value='" . $row["room_id"] . (($row["room_id"] == $default_room_id) ? "' selected='selected' >" : "' > " ) . $row["room_number"] . "</option>\n");
		}
		
		echo("</select>\n");
		echo ("
			</td>\n");
		echo "</tr>\n";

         	echo("<tr><td align='right'>Wait Room Building $j:");
		if ($edit_conflict){
			echo "<br /><b class='black'>(" . $interview2->wait_rooms[$i] . ")</b>";
		}
		echo ("</td>\n");
	        echo("<td><select name='wait_building$i' onchange='change_rooms(\"$i\", \"wait_building\", \"wait_room\")'>\n");	
		echo( "<option value=''" . (($default_wait_building_id == "") ? "selected='selected'" : "") . " >T . B . A</option>\n" );
		
		$result = $GLOBALS['dbh']->Execute( "SELECT DISTINCT b.building_id as building_id, b.building_code as building_code
					FROM room as a, building as b
					WHERE a.building_id=b.building_id" );
		while( ($row = $result->FetchRow()) )	
		{
			echo( "<option value='" . $row['building_id'] . ( ($row["building_id"] == $default_wait_building_id) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>\n" );
		}
		
		echo "</select></td><td align='right'>Room $j:</td>\n";
		echo ("<td><select name='wait_room$i'>\n");
		//echo ("<option value=''>T . B. A</option>\n" );
        	
		$result = $GLOBALS['dbh']->Execute("SELECT room_id, room_number
					FROM room
					WHERE building_id = '" . $default_wait_building_id . "'");
		if ($result->RecordCount() <= 0){
			echo "<option value='' selected='selected'>T . B . A</option>\n";
		}
		
		while ($row = $result->FetchRow())
		{
			echo( "<option value='" . $row["room_id"] . (($row["room_id"] == $default_wait_room_id) ? "' selected='selected' >" : "' > " ) . $row["room_number"] . "</option>\n");
		}
        	
		echo("</select>\n");

		echo ("</td></tr>\n");
	}

      }
}

if( !isOnCampus($interview->interview_place) && !isPhone($interview->interview_medium) )
{
	echo("<tr><td align='right'>Interview Location</td><td colspan='3'><table width='100%' cellpadding='3' cellspacing='0'>");
	include('interview/add/locationForm.inc');
	echo("</table></td></tr>");
}
else
{

?>
  <tr>
    <td align='center' colspan='4'><input type='submit' name='submit' value='Multiple Rooms' /></td>
  </tr>

<?php
}
?>

  <tr>
    <td colspan="4"><hr width="100%" /></td>
  </tr>

<?php
if ($multiple)
{
?>
	<tr>
	  <td colspan="4" align="center"><b class="instruct">Warning:</b><b class="black"> This interview has mutiple rooms associated to it per interviewer.  By clicking "Save the changes" on this page all interview times will be replaced with the rooms selected above.</b></td>
	</tr>

<?php
}
?>
  <tr>
    <td align="center" colspan="4"><input type="submit" name="submit" value="Save the changes" /></td>
 </tr>
</table>
</form>
