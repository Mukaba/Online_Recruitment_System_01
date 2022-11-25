$(document).ready(function() {
    //State and City Stuff
    $.ajax({
            url: 'js/locations.json',
            type: 'GET',
            dataType: 'json'
        })
        .done(function(error) {
            mydata = error;
            for (var state in mydata.States) {
                $('#reg_state_of_origin').append('<option value="' + String(state) + '">' + String(state) + ' </option>');
            }
        })
    $('#reg_state_of_origin').change(function(event) {
        var selectedState = $(this).val();
        $('#reg_lga').empty();
        //Get cities for selectedState
        var cities = mydata.States[String(selectedState)];
        $('#reg_lga').append('<option value="" selected disabled>Please select</option>');

        for (var i = 0; i < cities.length; i++) {
            $('#reg_lga').append('<option value="' + String(cities[i]) + '">' + String(cities[i]) + ' </option>');
        }
    });


    $.ajax({
            url: 'js/locations.json',
            type: 'GET',
            dataType: 'json'
        })
        .done(function(error) {
            mydata = error;
            for (var state in mydata.States) {
                $('#reg_state_of_residence').append('<option value="' + String(state) + '">' + String(state) + ' </option>');
            }
        })


    //Load Institution
    $.ajax({
            url: 'js/institution.json',
            type: 'GET',
            dataType: 'json'
        })
        .done(function(error) {
            myinstitution = error;
            for (var institution in myinstitution.Institution) {
                $('#reg_type_of_institution').append('<option value="' + String(institution) + '">' + String(institution) + ' </option>');
                $('#type_of_institution').append('<option value="' + String(institution) + '">' + String(institution) + ' </option>');
            }
        })
    $('#reg_type_of_institution').change(function(event) {
        var selectedInstitution = $(this).val();
        $('#reg_university').empty();
        //Get schools for selectedInstitution
        var institution = myinstitution.Institution[String(selectedInstitution)];
        $('#reg_university').append('<option value="" selected disabled>Please select</option>');

        for (var i = 0; i < institution.length; i++) {
            $('#reg_university').append('<option value="' + String(institution[i]) + '">' + String(institution[i]) + ' </option>');
        }
    });
    $('#type_of_institution').change(function(event) {
        var selectedInstitution = $(this).val();
        $('#university').empty();
        //Get schools for selectedInstitution
        var institution = myinstitution.Institution[String(selectedInstitution)];
        $('#university').append('<option value="" selected disabled>Please select</option>');

        for (var i = 0; i < institution.length; i++) {
            $('#university').append('<option value="' + String(institution[i]) + '">' + String(institution[i]) + ' </option>');
        }
    });

});
