import config from '../config';

//Pages
import Home from '../pages/Home';
import History from '../pages/History';

//public routes
const publicRoutes = [
    { path: config.routes.home, component: Home },
    { path: config.routes.history, component: History },
];

export { publicRoutes };
