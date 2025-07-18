import React, { useEffect, useState } from 'react';

type Task = {
    id: string | number;
    title: string;
    done: boolean;
};

function App() {
    const [tasks, setTasks] = useState<Task[]>([]);
    const [title, setTitle] = useState('');

    // Načti úkoly při startu
    useEffect(() => {
        fetch('/api/tasks')
            .then(res => res.json())
            .then(setTasks)
            .catch(() => setTasks([]));
    }, []);

    // Přidat nový úkol
    const addTask = async (e: React.FormEvent) => {
        e.preventDefault();
        if (!title.trim()) return;
        const res = await fetch('/api/tasks', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ title })
        });
        if (res.ok) {
            const task: Task = await res.json();
            setTasks([...tasks, task]);
            setTitle('');
        }
    };

    // Smazat úkol
    const deleteTask = async (id: string | number) => {
        await fetch(`/api/tasks/${id}`, { method: 'DELETE' });
        setTasks(tasks.filter(t => t.id !== id));
    };

    // Označit jako hotovo
    const toggleTask = async (task: Task) => {
        const updated = { ...task, done: !task.done };
        await fetch(`/api/tasks/${task.id}`, {
            method: 'PATCH',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(updated)
        });
        setTasks(tasks.map(t => t.id === task.id ? updated : t));
    };

    return (
        <div style={{ maxWidth: 400, margin: '2rem auto', fontFamily: 'sans-serif' }}>
            <h2>Úkoly</h2>
            <form onSubmit={addTask} style={{ display: 'flex', marginBottom: 16 }}>
                <input
                    type="text"
                    value={title}
                    placeholder="Nový úkol..."
                    onChange={e => setTitle(e.target.value)}
                    style={{ flex: 1, marginRight: 8 }}
                />
                <button type="submit">Přidat</button>
            </form>
            <ul style={{ listStyle: 'none', padding: 0 }}>
                {tasks.map(task => (
                    <li key={task.id} style={{ marginBottom: 8 }}>
                        <input
                            type="checkbox"
                            checked={task.done}
                            onChange={() => toggleTask(task)}
                            style={{ marginRight: 8 }}
                        />
                        <span style={{ textDecoration: task.done ? 'line-through' : undefined }}>
              {task.title}
            </span>
                        <button
                            onClick={() => deleteTask(task.id)}
                            style={{ marginLeft: 12 }}
                        >
                            Smazat
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}

export default App;
