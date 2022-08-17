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
 | Filename: calendar.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: This file opens up a new window that displays a                 |
 | calendar from which you can choose a date.                                   |
 +------------------------------------------------------------------------------+
*/

/*
 File originally created by Chris, and modified by Adam to accept the form name
 and input name as parameters.

 Also, the hidden variable index is the index number of the element that we are filling
 with the chosen date, in the parent form.
*/

?>

<script type='text/javascript' language="javascript">
//<![CDATA[
var month;
var year;

function initialize()
{
	month=document.myform.month.value;
	year=document.myform.year.value;
	window.focus();
}

function unfocus()
{
	self.close();
}

function filldate(theday)
{
	var x = document.myform.index.value;
	var formName = document.myform.parentFormName.value;
	var dateFill = "";

	if (theday.length>0)
    {
		if (parseInt(month)<10)
        {
			if (parseInt(theday)<10)
            {
				dateFill = year + "-0" + month + "-0" + theday;
			}
            else
            {
				dateFill = year + "-0" + month + "-" + theday;
			}
		}
        else
        {	
			if (parseInt(theday)<10)
            {
				dateFill = year + "-" + month + "-0" + theday;
			}
            else
            {
				dateFill = year + "-" + month + "-" + theday;
			}
		}
	}

	str = "window.opener.document." + formName + ".elements[" + x + "].value = dateFill";
    eval(str);

	self.close();
}

function fillfirstofmonth()
{
	var x = document.myform.index.value;
	var formName = document.myform.parentFormName.value;

	var dateFill = year + "-" + month + "-01";

	str = "window.opener.document." + formName + ".elements[" + x + "].value = dateFill";
	eval(str);

	self.close();
}

function nextmonth()
{
	var nextmonth;
	var firstday;
	var x=new Array(42);
	var months=new Array(12);

	months[1]="January";
	months[2]="February";
	months[3]="March";
	months[4]="April";
	months[5]="May";
	months[6]="June";
	months[7]="July";
	months[8]="August";
	months[9]="September";
	months[10]="October";
	months[11]="November";
	months[12]="December";

	if (parseInt(document.myform.month.value) != 12)
    {
		nextmonth=parseInt(document.myform.month.value)+1;
	}
    
    else
    {
		document.myform.year.value=parseInt(document.myform.year.value)+1;
		year=parseInt(year)+1;
		nextmonth=1;
	}
    
	month=nextmonth;
	document.myform.month.value=nextmonth;
	document.myform.thismonth.value=months[nextmonth]+" "+document.myform.year.value;
	if (nextmonth<=7)
    {
		if (nextmonth%2 != 0)
        {
			daysinmonth=31;
		}
        
        else
        {
			if (nextmonth==2)
            {
				if (parseInt(document.myform.year.value)%4==0 && parseInt(document.myform.year.value)%100 != 0)
                {
					daysinmonth=29;
				}
                
                else
                {
					if (document.myform.year.value%100==0)
                    {
						if (document.myform.year.value%400==0)
                        {
							daysinmonth=29;
						}
                        else
                        {
							daysinmonth=28;
						}
					}
                    else
                    {
						daysinmonth=28;
					}
				}
			}
            
            else
            {
				daysinmonth=30;
			}
		}
	}
    
    else
    {
		if (nextmonth%2==0)
        {
			daysinmonth=31;
		}
        
        else
        {
			daysinmonth=30;
		}
	}
    
	if (parseInt(document.myform.finaldayofweek.value)==6)
    {
		firstday=0;
	}
    
    else
    {
		temp1= parseInt(document.myform.finaldayofweek.value);
		firstday=temp1+1;
	}
    
	for (i=0;i<=firstday;i++)
    {
		x[i]="";
	}
    
	weekday=firstday;
	for(i=0;i<daysinmonth;i++)
    {
		x[i+firstday+1]=i+1;
		if (weekday != 6)
        {
			weekday=weekday+1;
		}
        
        else
        {
			weekday=0;
		}
        
		if (daysinmonth==i+1)
        {
			if (weekday != 0)
            {
				document.myform.finaldayofweek.value=weekday-1;	
			}
            else
            {
				document.myform.finaldayofweek.value=6;
			}
		}
	}
    
	for (i=daysinmonth+firstday+1;i<42;i++)
    {
		x[i]="";
	}
    
	i=0;
	while (document.myform.elements[i].name !="square0")
    {
		index=i;
		i=i+1;
	}
    
	for (i=0;i<42;i++)
    {
		document.myform.elements[index+i].value=x[i];
	}	
	document.myform.firstdayofweek.value=firstday;
}

function previousMonth()
{
    var previousmonth;
    var lastday;
	var daysinmonth;
    var x=new Array(42);
    var months=new Array(12);

    months[1]="January";
    months[2]="February";
    months[3]="March";
    months[4]="April";
    months[5]="May";
    months[6]="June";
    months[7]="July";
    months[8]="August";
    months[9]="September";
    months[10]="October";
    months[11]="November";
    months[12]="December";

    if (parseInt(document.myform.month.value) != 1)
    {
        previousmonth=parseInt(document.myform.month.value)-1;
    }
    
    else
    {
        document.myform.year.value=parseInt(document.myform.year.value)-1;
        year=parseInt(year)-1;
        previousmonth=12;
    }
    
    document.myform.month.value=previousmonth;
	month=previousmonth;
    document.myform.thismonth.value=months[previousmonth]+" "+document.myform.year.value;
    if (previousmonth<=7)
    {
        if (previousmonth%2 != 0)
        {
            daysinmonth=31;
        }
        
        else
        {
            if (previousmonth==2)
            {
                if (parseInt(document.myform.year.value)%4==0 && parseInt(document.myform.year.value)%100 != 0)
                {
                    daysinmonth=29;
                }
                
                else
                {
                    if (document.myform.year.value%100==0)
                    {
                        if (document.myform.year.value%400==0)
                        {
                            daysinmonth=29;
                        }
                        
                        else
                        {
                            daysinmonth=28;
                        }
                    }
                    
                    else
                    {
                        daysinmonth=28;
                    }
                }
            }
            
            else
            {
                daysinmonth=30;
            }
        }
    }
    
    else
    {
        if (previousmonth%2==0)
        {
            daysinmonth=31;
        }
        
        else
        {
            daysinmonth=30;
        }
    }
    
    if (parseInt(document.myform.firstdayofweek.value)==0)
    {
        lastday=6;
		position=34;
    }
    
    else
    {
        temp1= parseInt(document.myform.firstdayofweek.value);
        lastday=temp1-1;

		if (lastday==1)
        {
			if (daysinmonth>30)
            {
				position=36;
			}
            
            else
            {
				position=29;
			}
		}
        
        else
        {
			if (lastday==0)
            {
				position=35;
			}
            
            else
            {
				position=28+lastday;
			}
		}
    }

	document.myform.finaldayofweek.value=lastday;
	tempvar=daysinmonth;
    weekday=lastday;
	
    for(i=position+1;i<43;i++)
    {
		x[i]="";
	}

    for(i=daysinmonth;i>0;i--)
    {
        x[position]=i;
        position=position-1;
        if (weekday != 0)
        {
            weekday=weekday-1;
        }
        
        else
        {
            weekday=6;
        }
        
        if (i==1)
        {
            if (weekday != 6)
            {
                document.myform.firstdayofweek.value=weekday+1; 
            }
            
            else
            {
                document.myform.firstdayofweek.value=0;
            }
        }
    }
    
	for(i=position;i>=0;i--)
    {
		x[i]="";
	}

    i=0;
    while (document.myform.elements[i].name !="square0")
    {
        index=i+1;
        i=i+1;
    }
    
    for (i=0;i<42;i++)
    {
        document.myform.elements[index+i].value=x[i];
    }       
}

function nextyear()
{
	monthcount=0

	var nextmonth;
	var firstday;
	var x=new Array(42);
	var months=new Array(12);

	months[1]="January";
	months[2]="February";
	months[3]="March";
	months[4]="April";
	months[5]="May";
	months[6]="June";
	months[7]="July";
	months[8]="August";
	months[9]="September";
	months[10]="October";
	months[11]="November";
	months[12]="December";

	while (monthcount!=12)
    {
        if (parseInt(document.myform.month.value) != 12)
        {
            nextmonth=parseInt(document.myform.month.value)+1;
        }
        
        else
        {
            document.myform.year.value=parseInt(document.myform.year.value)+1;
            year=parseInt(year)+1;
            nextmonth=1;
        }

        month=nextmonth;
        document.myform.month.value=nextmonth;
        document.myform.thismonth.value=months[nextmonth]+" "+document.myform.year.value;

        if (nextmonth<=7)
        {
            if (nextmonth%2 != 0)
            {
                daysinmonth=31;
            }
            
            else
            {
                if (nextmonth==2)
                {
                    if (parseInt(document.myform.year.value)%4==0 && parseInt(document.myform.year.value)%100 != 0)
                    {
                        daysinmonth=29;
                    }
                    
                    else
                    {
                        if (document.myform.year.value%100==0)
                        {
                            if (document.myform.year.value%400==0)
                            {
                                daysinmonth=29;
                            }
                            
                            else
                            {
                                daysinmonth=28;
                            }
                        }
                        
                        else
                        {
                            daysinmonth=28;
                        }
                    }
                }
                
                else
                {
                    daysinmonth=30;
                }
            }
        }
        
        else
        {
            if (nextmonth%2==0)
            {
                daysinmonth=31;
            }
            
            else
            {
                daysinmonth=30;
            }
        }

        if (parseInt(document.myform.finaldayofweek.value)==6)
        {
            firstday=0;
        }
        
        else
        {
            temp1= parseInt(document.myform.finaldayofweek.value);
            firstday=temp1+1;
        }

        for (i=0;i<=firstday;i++)
        {
            x[i]="";
        }

        weekday=firstday;
        for(i=0;i<daysinmonth;i++)
        {
            x[i+firstday+1]=i+1;

            if (weekday != 6)
            {
                weekday=weekday+1;
            }
            
            else
            {
                weekday=0;
            }
            if (daysinmonth==i+1)
            {
                if (weekday != 0)
                {
                    document.myform.finaldayofweek.value=weekday-1;	
                }
                
                else
                {
                    document.myform.finaldayofweek.value=6;
                }
            }
        }
        for (i=daysinmonth+firstday+1;i<42;i++)
        {
            x[i]="";
        }
        
        i=0;
        while (document.myform.elements[i].name !="square0")
        {
            index=i;
            i=i+1;
        }

        for (i=0;i<42;i++)
        {
            document.myform.elements[index+i].value=x[i];
        }	

        document.myform.firstdayofweek.value=firstday;
        monthcount=monthcount+1;
    }
}


function previousyear()
{
    monthcount=0;
    var previousmonth;
    var lastday;
    var daysinmonth;
    var x=new Array(42);
    var months=new Array(12);

    months[1]="January";
    months[2]="February";
    months[3]="March";
    months[4]="April";
    months[5]="May";
    months[6]="June";
    months[7]="July";
    months[8]="August";
    months[9]="September";
    months[10]="October";
    months[11]="November";
    months[12]="December";

    while (monthcount!=12)
    {
        if (parseInt(document.myform.month.value) != 1)
        {
            previousmonth=parseInt(document.myform.month.value)-1;
        }
        
        else
        {
            document.myform.year.value=parseInt(document.myform.year.value)-1;
            year=parseInt(year)-1;
            previousmonth=12;
        }

        document.myform.month.value=previousmonth;
        month=previousmonth;
        document.myform.thismonth.value=months[previousmonth]+" "+document.myform.year.value;
        if (previousmonth<=7)
        {
            if (previousmonth%2 != 0)
            {
                daysinmonth=31;
            }
            
            else
            {
                if (previousmonth==2)
                {
                    if (parseInt(document.myform.year.value)%4==0 && parseInt(document.myform.year.value)%100 != 0)
                    {
                        daysinmonth=29;
                    }
                    
                    else
                    {
                        if (document.myform.year.value%100==0)
                        {
                            if (document.myform.year.value%400==0)
                            {
                                daysinmonth=29;
                            }
                            
                            else
                            {
                                daysinmonth=28;
                            }
                        }
                        
                        else
                        {
                            daysinmonth=28;
                        }
                    }
                }
                
                else
                {
                    daysinmonth=30;
                }
            }
        }
        
        else
        {
            if (previousmonth%2==0)
            {
                daysinmonth=31;
            }

            else
            {
                daysinmonth=30;
            }
        }

        if (parseInt(document.myform.firstdayofweek.value)==0)
        {
            lastday=6;
            position=34;
        }
        
        else
        {
            temp1= parseInt(document.myform.firstdayofweek.value);
            lastday=temp1-1;

            if (lastday==1)
            {
                if (daysinmonth>30)
                {
                    position=36;
                }
                
                else
                {
                    position=29;
                }
            }
            
            else
            {
                if (lastday==0)
                {
                    position=35;
                }
                
                else
                {
                    position=28+lastday;
                }
            }
        }

        document.myform.finaldayofweek.value=lastday;
        tempvar=daysinmonth;
        weekday=lastday;

        for(i=position+1;i<43;i++)
        {
            x[i]="";
        }

        for(i=daysinmonth;i>0;i--)
        {
            x[position]=i;
            position=position-1;

            if (weekday != 0)
            {
                weekday=weekday-1;
            }

            else
            {
                weekday=6;
            }

            if (i==1)
            {
                if (weekday != 6)
                {
                    document.myform.firstdayofweek.value=weekday+1; 
                }
                
                else
                {
                    document.myform.firstdayofweek.value=0;
                }
            }
        }

        for(i=position;i>=0;i--)
        {
            x[i]="";
        }

        i=0;
        while (document.myform.elements[i].name !="square0")
        {
            index=i+1;
            i=i+1;
        }

        for (i=0;i<42;i++)
        {
            document.myform.elements[index+i].value=x[i];
        }

        monthcount=monthcount+1;       
    }
}
//]]>
</script>
<?php

$todaysMonth=date("m");
$todaysYear=date("Y");
$timeStamp=mktime(0,0,0,$todaysMonth,1,$todaysYear);
$dateArray=getdate($timeStamp);

if ($dateArray['mon']<=7)
{
    if ($dateArray['mon']%2==0)
    {
        if ($dateArray['mon']==2)
        {
            if (($dateArray['year'])%4==0 && ($dateArray['year']%100<>0))
            {
                $daysinmonth=29;
            }
            
            else
            {
                if (($dateArray['year'])%400==0)
                {
                    $daysinmonth=29;
                }
                
                else
                {
                    $daysinmonth=28;
                }
            }
        }
        
        else
        {
            $daysinmonth=30;
        }
    }
    
    else
    {
        $daysinmonth=31;
    }
}

else
{
    if ($dateArray['mon']%2==0)
    {
        $daysinmonth=31;
    }
    
    else
    {
        $daysinmonth=30;
    }	
}
?>
<form name="myform" action="">
<table align="center" cellpadding="0" cellspacing="0" border="1">
    <tr>
        <td class="row1" align="center"><input type="button" value="&lt;" onclick="previousMonth()" /></td>
        <td class="row1" align="center" colspan="5"><input type="text" onfocus="fillfirstofmonth()" name="thismonth" size="20" value="<?php echo $dateArray['month']." ".$dateArray['year']?>" /></td>
        <td class="row1" align="center"><input type="button" value="&gt;" onclick="nextmonth()" /></td>
    </tr>

    <tr>
        <td class="row0" align="center"><input type="text" size='3' value="S" onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="M " onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="T " onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="W" onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="T " onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="F " onfocus="unfocus()" /></td>
        <td class="row0" align="center"><input type="text" size='3' value="S " onfocus="unfocus()" /></td>
    </tr>
<?php
$count=$dateArray['wday'];
for ($i=0;$i<$daysinmonth;$i++)
{
	if ($i+1>=10)
    {
		$days[$count+$i]=$i+1;
	}
    
    else
    {
		$temp=$i+1;
		$days[$count+$i]=$temp."  ";
	}
}
?>
		
<?php
$dayofweek=0;
for ($i=0;$i<42;$i++)
{
	$dayofweek=$dayofweek+1;
	if ($i%7==0)
    {
		echo "<tr>";
		$dayofweek=0;
	}
	if ($days[$i]==NULL)
    {
?>
        <td class="row0"><input type="text" size="3"  name="square<?php echo $i?>" onfocus="filldate(document.myform.<?php echo "square".$i?>.value)" /></td>
<?php
	}
    
    else
    {
        if ($days[$i]==$daysinmonth)
        {
            $finaldayofweek=$dayofweek;
        }
?>
        <td class="row0"><input type="text" size="3" onfocus="filldate(document.myform.<?php echo "square".$i?>.value)" name="square<?php echo $i?>" value="<?php echo $days[$i]?>" /></td>
<?php
	}
    
	if (($i+1)%7==0)
    {
		echo "</tr>";
	}
}
?>

</table>
<input type="hidden" name="month" value="<?php echo $dateArray['mon']?>" />
<input type="hidden" name="year" value="<?php echo $dateArray['year']?>" />
<input type="hidden" name="j" />
<input type="hidden" name="finaldayofweek" value="<?php echo $finaldayofweek?>" />
<input type="hidden" name="firstdayofweek" value="<?php echo $dateArray['wday']?>" />
<input type="hidden" name="index" value="<?php echo $index?>" />
<input type="hidden" name="parentFormName" value="<?php echo($parentFormName); ?>" />
<br />
<center>
    <table>
        <tr>
            <td align="center"><input type="button" value="Previous Year" onclick="previousyear()" /></td>
            <td><input type="button" value="Next Year" onclick="nextyear()" /></td>
        </tr>
        <tr>
            <td align="center" colspan="2">Today is <?php echo date("F d")?></td>
        </tr>
    </table>
</center>
</form>

<script type="text/javascript">
//<![CDATA[
initialize();
//]]>
</script>
