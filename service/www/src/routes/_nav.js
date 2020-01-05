import SystemPage from '../views/System/SystemPage';

export default function useNavigation() {
    return {
        items: defaultMenu,
    };
}

const defaultMenu = [{component: SystemPage}];
