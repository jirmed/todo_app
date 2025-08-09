// Simple script to test PATCH request
// Run with: node test-patch.js

const taskId = 2; // Replace with an actual task ID from your database
const updatedTask = {
  id: taskId,
  title: "Test Task",
  done: true
};

// Using fetch to make a PATCH request
fetch(`/api/tasks/${taskId}`, {
  method: 'PATCH',
  headers: {
    'Content-Type': 'application/json'
  },
  body: JSON.stringify(updatedTask)
})
.then(response => {
  console.log('Response status:', response.status);
  return response.json();
})
.then(data => {
  console.log('Response data:', data);
})
.catch(error => {
  console.error('Error:', error);
});