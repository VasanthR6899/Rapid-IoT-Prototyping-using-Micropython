		var config = {
			type: 'line',
			data: {
				labels: [],
				datasets: [{
					label: 'Temperature',
					backgroundColor: window.chartColors.red,
					borderColor: window.chartColors.red,
					data: [],
					fill: false,
				}]
			},
			options: {
				responsive: true,
				title: {
					display: true,
					text: 'Microchip Temperature Demo'
				},
				tooltips: {
					mode: 'index',
					intersect: false,
				},
				hover: {
					mode: 'nearest',
					intersect: true
				},
				scales: {
					x: { // Corrected scale configuration for x-axis
						display: true,
							title: {
							display: true,
						text: 'Time'
						}
					},
					y: { // Corrected scale configuration for y-axis
						display: true,
						title: {
							display: true,
							text: 'Temperature - °F'
						}
					}
				}
			}
		};
		


		var config2 = {
			type: 'line',
			data: {
				labels: [],
				datasets: [{
					label: 'Light',
					backgroundColor: window.chartColors.blue,
					borderColor: window.chartColors.blue,
					data: [],
					fill: false,
				}]
			},
			options: {
				responsive: true,
				title: {
					display: true,
					text: 'Microchip Brightness Demo'
				},
				tooltips: {
					mode: 'index',
					intersect: false,
				},
				hover: {
					mode: 'nearest',
					intersect: true
				},
				scales: {
					x: { // Corrected scale configuration for x-axis
						display: true,
						title: {
							display: true,
							text: 'Time'
						}
					},
					y: { // Corrected scale configuration for y-axis
						display: true,
						title: {
							display: true,
							text: 'Light - %'
						}
					}
					
				}
			}
		};
		
				

		window.onload = function() {
			var ctx = document.getElementById('temperatureChart').getContext('2d');
			var ctx2 = document.getElementById('LightChart').getContext('2d');
			
			window.myLine = new Chart(ctx, config);				
			window.myLine2 = new Chart(ctx2, config2);
			
		};
		
function addData(chart, label, data) {
		chart.data.labels.push(label); // Add the new timestamp to the labels array
		chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data); // Add the new temperature data to the dataset
    });
    chart.update(); // Update the chart to reflect the new data
	
}


function resetChart(chart) {
    // Clear the existing data points and labels
    chart.data.labels = [];
    chart.data.datasets.forEach((dataset) => {
        dataset.data = [];
    });    
    // Update the chart to reflect the changes
    chart.update();
}

let stringArray = ["No Device", "RNWF_001", "RNWF_002", "RNWF_003", "RNWF_004", "RNWF_005", "RNWF_006", "RNWF_007", "RNWF_008", "RNWF_009","RNWF_010",
"RNWF_011", "RNWF_012", "RNWF_013", "RNWF_014", "RNWF_015", "RNWF_016", "RNWF_017", "RNWF_018", "RNWF_019","RNWF_020",
"RNWF_021", "RNWF_022", "RNWF_023", "RNWF_024", "RNWF_025", "RNWF_026", "RNWF_027", "RNWF_028", "RNWF_029","RNWF_030",
"RNWF_031", "RNWF_032", "RNWF_033", "RNWF_034", "RNWF_035", "RNWF_036", "RNWF_037", "RNWF_038", "RNWF_039","RNWF_040"];

function updateDropdown() {
  const dropdown = document.getElementById('dropdown');
  dropdown.innerHTML = ''; // Clear existing options

  stringArray.forEach((str) => {
    const option = document.createElement('option');
    option.value = str;
    option.textContent = str;
    dropdown.appendChild(option);
  });
}



		
