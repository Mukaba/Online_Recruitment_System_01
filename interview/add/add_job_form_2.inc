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
 | Filename: add_job_form_2.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: The second form to fill out to add a job this code determines   |
 | the number of interviewers and shows the appropriate number of data entry    |
 | boxes. data to be entered includes interviewer's name, interview location    |
 | and other notes                                                              |
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
	str = "document.myform.locations" + loc_number + ".selectedIndex == document.myform.wait_building" + loc_number + ".selectedIndex";
	var change1 = eval(str);

	if (change1){	
		str = "document.myform.wait_room" + loc_number + ".selectedIndex = document.myform.room" + loc_number + ".selectedIndex";
		eval(str);
	}
}

function setbuilding(loc_number, room_index){
	if (room_index < 0 || room_ids[room_index].length <= 0){
		str = "document.myform.wait_room" + loc_number + ".options.length=1";
		eval(str);
		str = "document.myform.wait_room" + loc_number + ".options[0] = new Option(\"T . B . A\", \"\")";
		eval(str);
	}else{
		str = "document.myform.wait_room" + loc_number + ".length = " + room_ids[room_index].length;
		eval(str);	
		str = "document.myform.wait_room" + loc_number + ".options.length = " + room_ids[room_index].length;
		eval(str);
		for(i=0;i<room_ids[room_index].length;i++){
			str = "document.myform.wait_room" + loc_number + ".options[" + i +"] = new Option(\"" + rooms[room_index][i] + "\", \"" + room_ids[room_index][i] + "\")";
			eval(str);
		}
	}
	str = "document.myform.wait_building" + loc_number + ".selectedIndex = document.myform.locations" + loc_number + ".selectedIndex";
	eval(str);
	str = "document.myform.wait_room" + loc_number + ".selectedIndex = document.myform.room" + loc_number + ".selectedIndex";
	eval(str);
}

function change_rooms(loc_number, select_box_name, change_name){
	var str = "document.myform." + select_box_name + loc_number + "[document.myform." + select_box_name + loc_number + ".selectedIndex].value";
	var num = eval(str);
	var i = 0;
	var room_index = -1;
	for(i=0;i<building_ids.length;i++){
		if (building_ids[i] == num){
			room_index = i;
			break;
		}
	}
	if (room_index < 0){
		str = "document.myform." + change_name + loc_number + ".options.length = 1";
		eval (str);
		str = "document.myform." + change_name + loc_number + ".options[0] = new Option(\"T . B . A\", \"\")";
		eval (str);
	}else{
		str = "document.myform." + change_name + loc_number + ".length = " + room_ids[room_index].length;
		eval (str);
		str = "document.myform." + change_name + loc_number + ".options.length = " + room_ids[room_index].length;
		eval (str);
		if (room_ids[room_index].length == 0){
			str = "document.myform." + change_name + loc_number + ".options.length = 1";
			eval (str);
			str = "document.myform." + change_name + loc_number + ".options[0] = new Option(\"T . B . A\", \"\")";
			eval(str);
		}
		for(i=0;i<room_ids[room_index].length;i++){
			str = "document.myform." + change_name + loc_number + ".options[" + i +"] = new Option(\"" + rooms[room_index][i] + "\", \"" + room_ids[room_index][i] + "\")";
			eval(str);
		}
	}
	str = "document.myform." + change_name + loc_number + ".selectedIndex=0";
	eval(str);
	
	if (select_box_name == "locations"){
		setbuilding(loc_number, room_index);
	}
}
//-->
</script>
	
   <form method="post" name="myform" action="<?php echo($PHP_SELF); ?>">

   <input type="hidden" name="select" value="add" />
   <input type="hidden" name="interview" value="<?php echo( isset($interview) ? packObject(($interview)) : "");?>" />
   <center>
   <table border="0" cellpadding="4" cellspacing="0" class="row1" width="100%">

<?php
   for ($i = 0; $i < $interview->num_interviewers; $i++)
   {
      echo("<tr>
	      		<td align='right'>Interviewer" . (( $interview->num_interviewers > 1 )? ($i+1) . ":"  : ":" ) . "</td>
    	  		<td>
					<table border='0' cellpadding='4' cellspacing='0' class='row0'>
					<tr>
						<td align='right'>Name: <b class='red'>*</b></td>
						<td colspan='3'><input type='text' name='interviewers[$i]' value='{$interview->interviewers[$i]}' size='30' maxlength='100' /></td>
					</tr>
					");
	
		if( isOnCampus($interview->interview_place) || isPhone($interview->interview_medium) )
		{
			echo("
					<tr>
						<td align='right'>Interview Building:</td>
						<td>
							<select name='locations$i' onchange='change_rooms(\"$i\", \"locations\", \"room\")'>
							<option value=''>T . B . A</option>" );
						
						
		  	$sql = ("
				SELECT DISTINCT(b.building_id), b.building_code
				FROM building as b, room as r 
				WHERE r.building_id = b.building_id
                ORDER BY b.building_code
				");
			$result = $GLOBALS['dbh']->Execute($sql);

		  	while( ($row = $result->FetchRow()) )	
		  	{
				echo( "<option value='" . $row['building_id'] . ( ($row["building_id"] == $buildings[$i]) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>" );
			}
    	  		echo("				</select>
						</td>
					        <td align='right' >Room:</td>
						<td>
							<select name='room$i' size='1' onchange='changeRoom(\"$i\")'>
				");
							
							/*
							 If the user has already chosen a building from the before, but the screen reloaded
							 because they didn't fill out something, load the rooms that correspond to the 
							 chosen building.
							*/

							if ($buildings && $buildings[$i])
							{
								$sql = ("
									SELECT DISTINCT room_id, room_number
									FROM room
									WHERE building_id='" . $buildings[$i] . "'
									");
								$result = $GLOBALS['dbh']->Execute($sql);
								while ($row = $result->FetchRow())
								{
									echo("<option value='" . $row["room_id"] . "' ");
									if ($interview->locations[$i] == $row["room_id"])
									{
										echo("selected='selected'");
									}
									echo(">" . $row["room_number"] . "</option>");
								}
							}
							else
							{

							echo("
							    <option value=''>T . B . A</option> 
							    <option value=''>T . B . A</option>
							" );
							}
			
			echo("   		       </select>
						</td>
					</tr>");
			echo("							
					<tr>
						<td align='right'>Wait Room Building:</td>
						<td>
							<select size='1' name='wait_building$i' onchange='change_rooms(\"$i\", \"wait_building\", \"wait_room\")'>
								<option value=''>T . B . A</option>" );
						
						
		  	$result = $GLOBALS['dbh']->Execute(
                        "SELECT DISTINCT(b.building_id), b.building_code 
                        FROM building as b, room as r  
                        WHERE r.building_id = b.building_id
                        ORDER BY b.building_code") ; //a.room_id, CONCAT(b.building_code, ' ', a.room_number) AS room_code FROM room as a, building as b WHERE a.building_id=b.building_id ORDER BY b.building_code, a.room_number" );
		  	while( ($row = $result->FetchRow()) )	
		  	{
				echo( "				<option value='" . $row['building_id'] . ( ($row["building_id"] == $waitbuildings[$i]) ? "' selected='selected' >" : "' > " ) . $row['building_code'] . "</option>" );
	      		}
    	  		echo("				</select>
						</td>
					        <td align='right'>Room:</td>
						<td>
							<select name='wait_room$i' size='1'>
				");

                /*
                   If the user has already chosen a building from the before, but the screen reloaded
                   because they didn't fill out something, load the rooms that correspond to the
                   chosen building.
                 */

                if ($waitbuildings && $waitbuildings[$i])
                {
                    $sql = ("
                            SELECT DISTINCT room_id, room_number
                            FROM room
                            WHERE building_id='" . $waitbuildings[$i] . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    while ($row = $result->FetchRow())
                    {
                        echo("<option value='" . $row["room_id"] . "' ");
                        if ($interview->wait_locations[$i] == $row["room_id"])
                        {
                            echo("selected='selected'");
                        }
                        echo(">" . $row["room_number"] . "</option>");
                    }
                }
                else
                {

                    echo("
                            <option value=''>T . B . A</option>
                            <option value=''>T . B . A</option>
                            " );
                }
			
			echo("          		</select>
						</td>
					</tr>
					</table>
				</td></tr>");
		
		
		}
		else
			echo("		</table></td></tr>");
	}

 if (isPhone($interview->interview_medium))
 {
   ?>
     <tr>
       <td>&nbsp;</td>
       <td>
          <input type="checkbox" name="use_room_number" />If available, give the employer
             the phone number for the room
             that the telephone interview is in, rather than the default contact number.
       </td>
     </tr>
   <?php
 }

   	if ($interview->presentation == 1)
   	{
?>
         <tr>
         <td align="right">Presentation Information:</td>
         <td>
            <table border="0" cellpadding="4" cellspacing="0" class="row0">
            <tr>
            <td align="right">Date (yyyy-mm-dd): </td>
            <td><input type="text" name="pres_date" value="<?php echo($interview->pres_date); ?>" size="12" maxlength="10" /><?php echo popup('pres_date','myform')?></td>
            </tr>
            <tr>
            <td align="right">Time (24 hour clock):</td>
            <td><input type="text" name="pres_time" value="<?php echo($interview->pres_time); ?>" size="10" maxlength="8" /></td>
            </tr>
            <tr>
            <td align="right">Length:</td>
            <td><input type="text" name="pres_len" value="<?php echo($interview->pres_len); ?>" size="10" maxlength="3" />&nbsp;minutes</td>
            </tr>

            <tr>
            	<td align="right">Location:</td>
            	<td><input type="text" name="pres_loc" value="<?php echo($interview->pres_loc); ?>" size="30" maxlength="60" /></td>
            </tr>
            <tr>
            	<td align="right">Notes</td>
      			<td><textarea name="pres_notes" rows="3" cols="30"><?php echo($interview->pres_notes); ?></textarea></td>
            </tr>
	    <tr>
		<td align='right'>Is this presentation viewable by all students, or only those shortlisted?</td>
		<td>
			<input class='row2' type="radio" name="pres_viewable_by_all" value="1" <?php 
			if ( !isset($interview->pres_viewable_by_all) || (($interview) && $interview->pres_viewable_by_all == 1) ) 
			{ 
				echo(" checked='checked'"); 
			}
			?> />All&nbsp;&nbsp;&nbsp;&nbsp;
			<input class='row2' type="radio" name="pres_viewable_by_all" value="0" <?php 
			if ((isset($interview->pres_viewable_by_all)) && $interview->pres_viewable_by_all == 0)
			{
				echo(" checked='checked'");
			}
			?> />Shortlisted&nbsp;&nbsp;&nbsp;&nbsp;
		</td>
	    </tr>

            </table></td></tr>
<?php
   }
	/*** off campus not-phone interview ***/
	if( !(isOnCampus($interview->interview_place)) && !(isPhone($interview->interview_medium)))
	{
		include( 'interview/add/locationForm.inc' );			
	}
	echo("
	      <tr>
    		  <td align='right'>Notes:</td>
		      <td><textarea name='notes' rows=4 cols=55>$interview->notes</textarea></td>");
	echo("</tr>");
	echo("<tr>");
		echo("<td align='right'>Notes viewable only by Co-op staff:</td>");
		echo("<td><textarea name='staff_notes' rows='4' cols='55'>$interview->staff_notes</textarea></td>");
	echo("</tr>");

?>
<tr>
	<td colspan='2' align='center'><hr />Fields marked with an asterisk (<b class='red'>*</b>) are required for the default form settings</td>
</tr>
      <tr>
	      <td colspan="2" align='center'><hr />
    		<input type='hidden' name='submit' value='Calculate Times For Me' />
	      	<input type="submit" value="Continue" /></td></tr>
   </table></center>
   </form>
