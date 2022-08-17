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
 | Filename: edit_location_per_time_form.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This form comes up with all data previously entered showing,    |
 | the user has the opportunity to change the data.                             |
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
			str = "document.form1." + change_name + loc_number + ".options[" + i +"] = new Option(\"" + rooms[room_index][i] + "\", \"" + room_ids[room_index][i] + "\")";
			eval(str);
		}
	}
	str = "document.form1." + change_name + loc_number + ".selectedIndex=0";
	eval (str);

	if (select_box_name == "locations"){
		setbuilding(loc_number, room_index);
	}
}

function update_all(time_ids_array){
	var main_time_id = time_ids_array[0];
	var room_index = -1;
	var wait_room_index = -1;
	
	str = "document.form1.locations" + main_time_id + "[document.form1.locations"+ main_time_id + ".selectedIndex].value";
	var num = eval(str); 
	str = "document.form1.wait_building" + main_time_id + "[document.form1.wait_building"+ main_time_id + ".selectedIndex].value";
	var wait_num = eval(str); 
	for(i=0;i<building_ids.length;i++){
		if (building_ids[i] == num){
			room_index = i;
		}
		if (building_ids[i] == wait_num){
			wait_room_index = i;
		}
	}

	for (n=1;n<time_ids_array.length;n++){
		str = "document.form1.locations" + time_ids_array[n] +".selectedIndex = document.form1.locations" + main_time_id + ".selectedIndex";
		eval(str);
		change_rooms(time_ids_array[n], "locations", "room");
		str = "document.form1.wait_building" + time_ids_array[n] +".selectedIndex = document.form1.wait_building" + main_time_id + ".selectedIndex";
		eval(str);
		change_rooms(time_ids_array[n], "wait_building", "wait_room");
		str = "document.form1.room" + time_ids_array[n] + ".selectedIndex= document.form1.room"+ main_time_id + ".selectedIndex";
		eval(str);
		str = "document.form1.wait_room" + time_ids_array[n] + ".selectedIndex = document.form1.wait_room" + main_time_id + ".selectedIndex";
		eval(str);
	
	}
}
//-->
</script>


<form method="post" name="form1" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="edit" />
<input type="hidden" name="level1" value="location" />
<input type="hidden" name="job_id" value="<?php echo($job_id); ?>" />
<input type='hidden' name='interview' value='<?php echo ( packObject(($interview)) ); ?>' />

<?php

if( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) ){
	$job_id = $interview->job_id;
	echo "<table class='row1' cellspacing='0' cellpadding='4' border='0' width='100%'>\n";
	for ($i = 0; $i < count($interview->interviewers); $i++){
		echo "<tr>
			<td><b class='black'>Interviewer " . ($i+1) . ":&nbsp;&nbsp;" .  $interview->interviewers[$i] . "</b><hr width='100%' /></td>
		      </tr>\n";
		
		$result1 = $GLOBALS['dbh']->Execute("SELECT DISTINCT(int_date) as int_date
			FROM interview_time
			WHERE job_id = '" . $job_id . "' AND interviewer_id = '" . $interview->interviewer_ids[$i] . "'
			ORDER BY int_date");
		
	 	while ($row1 = $result1->FetchRow()){
			echo "<tr>
			        <td>" . formatLongDate($row1["int_date"]) . "</td>
			      </tr>";
			
			$result2 = $GLOBALS['dbh']->Execute("SELECT int_time, int_room_id, int_wait_room_id, time_id, int_type
					     FROM interview_time
					     WHERE interviewer_id = '" . $interview->interviewer_ids[$i] . "'
					     		AND job_id = '" . $job_id . "'
							AND int_date = '" . addslashes($row1["int_date"]) . "'
						ORDER BY int_time");
		
			echo "<tr>
			        <td align='center'>\n";
			echo "<table border='1' cellspacing='0' cellpadding='0'><tr><td>\n";
			echo "<table border='0' cellpadding='2'>\n";
			echo "<tr>
				<td class='rowgrey' align='center'><b class='white'>Time</b></td>
				<td class='rowgrey' align='center'><b class='white'>Interview Room</b></td>
				<td class='rowgrey' align='center'><b class='white'>Wait Room</b></td>
			      </tr>\n";
				
			$class="row0d"; 
			$count = 0;
			$js_time_ids_array = "new Array("; 
			while ($row2 = $result2->FetchRow()){
				echo "<tr>\n";

				switch( $row2["int_type"] ){
				  case INTERVIEW:
					$js_time_ids_array .= "\"" . $row2["time_id"] . "\",";
					$class="row0d";
					echo "<td class='$class' align='center'>&nbsp;" . $row2["int_time"] . "&nbsp;</td>";
	 				$result = $GLOBALS['dbh']->Execute("SELECT DISTINCT int_room_id, int_wait_room_id
	 				    			FROM interview_time
					     			WHERE time_id = '" . $row2["time_id"] . "'
					     				AND job_id = '" . $job_id . "'");
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
        	 
					/***********/

					$room_id = "room" . $row2["time_id"];
					$result2a = $GLOBALS['dbh']->Execute("SELECT b.building_code, r.room_number
								FROM building as b, room as r
								WHERE b.building_id = r.building_id AND
									r.room_id = '" . $$room_id . "'");
					$row2a = $result2a->FetchRow();
					$building_time = $row2a["building_code"];
					$room_time = $row2a["room_number"];
					
					$room_id = "wait_room" . $row2["time_id"];
					$result2a = $GLOBALS['dbh']->Execute("SELECT b.building_code, r.room_number
								FROM building as b, room as r
								WHERE b.building_id = r.building_id AND
									r.room_id = '" . $$room_id . "'");
					$row2a = $result2a->FetchRow();
					$wait_building_time = $row2a["building_code"];
					$wait_room_time = $row2a["room_number"];
					
					/************/

	        			echo("<td class='$class' align='center'>&nbsp;
						<select name='locations" . $row2["time_id"] . "' onchange='change_rooms(\"" . $row2["time_id"] . "\", \"locations\", \"room\")'>");
					echo( "<option value=''>T . B . A</option>\n" );
		
					$result = $GLOBALS['dbh']->Execute( "SELECT DISTINCT b.building_id as building_id, b.building_code as building_code
								FROM room as a, building as b
								WHERE a.building_id=b.building_id" );
					while( ($row = $result->FetchRow()) )	
					{	
						echo( "<option value='" . $row['building_id'] . ( ($row["building_id"] == $default_building_id) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>\n" );
					}
					echo "</select>\n";
				
					echo ("<select name='room" . $row2["time_id"] . "' onchange='changeRoom(\"" . $row2["time_id"] . "\")'>\n");
       	 		
					$result = $GLOBALS['dbh']->Execute("SELECT room_id, room_number
								FROM room
								WHERE building_id = '" . $default_building_id . "'");
					if ($result->RecordCount() <= 0){
						echo "<option value=''>T . B . A</option>\n";
					}
					while ($row = $result->FetchRow())
					{
						echo( "<option value='" . $row["room_id"] . (($row["room_id"] == $default_room_id) ? "' selected='selected' >" : "' > " ) . $row["room_number"] . "</option>\n");
					}
				
					echo("  </select>\n");
					if ($edit_conflict){
						echo "<br /><b class='black'>($building_time $room_time)</b>";
					}
					echo ("
					      &nbsp;</td>\n");
   				
		       		 	echo("<td class='$class' align='center'>&nbsp;
					         <select name='wait_building" . $row2["time_id"] . "' onchange='change_rooms(\"" . $row2["time_id"] . "\", \"wait_building\", \"wait_room\")'>\n");	
					echo( "<option value=''>T . B . A</option>\n" );
		
					$result = $GLOBALS['dbh']->Execute( "SELECT DISTINCT b.building_id as building_id, b.building_code as building_code
								FROM room as a, building as b
								WHERE a.building_id=b.building_id" );
					while( ($row = $result->FetchRow()) )	
					{
						echo( "<option value='" . $row['building_id'] . ( ($row["building_id"] == $default_wait_building_id) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>\n" );
					}
				
					echo "</select>\n";
					echo ("<select name='wait_room" . $row2["time_id"] . "'>\n");
       	 	
					$result = $GLOBALS['dbh']->Execute("SELECT room_id, room_number
								FROM room
								WHERE building_id = '" . $default_wait_building_id . "'");
					if ($result->RecordCount() <= 0){
						echo "<option value=''>T . B . A</option>\n";
					}
				
					while ($row = $result->FetchRow())
					{
						echo( "<option value='" . $row["room_id"] . (($row["room_id"] == $default_wait_room_id) ? "' selected='selected' >" : "' > " ) . $row["room_number"] . "</option>\n");
					}
       	 	
					echo("  </select>\n");
					if ($edit_conflict){
						echo "<br /><b class='black'>($wait_building_time $wait_room_time)</b>";
					}
					echo("
					      &nbsp;</td>
					      </tr>\n");
					break;
									
			  	  case LUNCH:
					$class="row1d";
					echo "<td class='$class' align='center'>&nbsp;" . $row2["int_time"] . "&nbsp;</td>";
			  		echo "<td class='$class' align='center'>&nbsp;Lunch&nbsp;</td>
					      <td class='$class' align='center'>&nbsp;Lunch&nbsp;</td>
					      </tr>";
					break;
			  
			 	   case INT_BREAK:
				   	$class="row1d";
					echo "<td class='$class' align='center'>&nbsp;" . $row2["int_time"] . "&nbsp;</td>";
			 		echo "<td class='$class' align='center'>&nbsp;Break&nbsp;</td>
					      <td class='$class' align='center'>&nbsp;Break&nbsp;</td>
					      </tr>";
					break;
				}
			}
			$js_time_ids_array = substr($js_time_ids_array, 0, strlen($js_time_ids_array) -1) . ")";
			echo "  </table></td></tr></table>
			      	<input type='button' onclick='javascript:update_all(" . $js_time_ids_array . ")' value='Set all rows same as first row' />
				</td>
			      </tr>\n";
			echo "<tr>
			        <td><br /></td>
			      </tr>";
		}
	}
}

if( !isOnCampus($interview->interview_place) && !isPhone($interview->interview_medium) )
{
	echo ("ERROR");
}


?>
  <tr>
    <td><hr width="100%" /></td>
  </tr>
  <tr>
    <td align="center"><input type="submit" name="submit" value="Save Room Changes" /></td>
 </tr>
</table>
</form>
